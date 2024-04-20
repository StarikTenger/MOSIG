/*****************************************************
    AUTHOR  : Sébastien Valat
    MAIL    : sebastien.valat@univ-grenoble-alpes.fr
    LICENSE : BSD
    YEAR    : 2021
    COURSE  : Parallel Algorithms and Programming
*****************************************************/

//////////////////////////////////////////////////////
//
// Goal: Implement 2D grid communication scheme with
//      8 neighbors using MPI types for non contiguous
//      side.
//
// SUMMARY:
//     - 2D splitting along X and Y
//     - 8 neighbors communications
//     - Blocking communications
// NEW:
//     - >>> MPI type for non contiguous cells <<<
//
//////////////////////////////////////////////////////

/****************************************************/
#include "src/lbm_struct.h"
#include "src/exercises.h"

/****************************************************/
void lbm_comm_init_ex5(lbm_comm_t * comm, int total_width, int total_height)
{
	//we use the same implementation than ex5 execpt for type creation
	lbm_comm_init_ex4(comm, total_width, total_height);

	MPI_Type_vector(comm->width,
                    DIRECTIONS, 
                    DIRECTIONS * comm->height, MPI_DOUBLE, 
					&comm->type);
	MPI_Type_commit(&comm->type);
}

/****************************************************/
void lbm_comm_release_ex5(lbm_comm_t * comm)
{
	//we use the same implementation than ex5 except for type destroy
	lbm_comm_release_ex4(comm);

	MPI_Type_free(&comm->type);
}


/****************************************************/
void lbm_comm_ghost_exchange_ex5(lbm_comm_t * comm, lbm_mesh_t * mesh)
{
	// ======== PREPARATION ========

	// Left-right borders
	double *border_l = lbm_mesh_get_cell(mesh, 1, 0);
	double *border_r = lbm_mesh_get_cell(mesh, comm->width - 2, 0);
	double *ghost_l = lbm_mesh_get_cell(mesh, 0, 0);
	double *ghost_r = lbm_mesh_get_cell(mesh, comm->width - 1, 0);

	// Corners
	double *corner_top_left = lbm_mesh_get_cell(mesh, 1, 1);
	double *corner_top_right = lbm_mesh_get_cell(mesh, comm->width - 2, 1);
	double *corner_bottom_left = lbm_mesh_get_cell(mesh, 1, comm->height - 2);
	double *corner_bottom_right = lbm_mesh_get_cell(mesh, comm->width - 2, 
													comm->height - 2);

	double *ghost_top_left = lbm_mesh_get_cell(mesh, 0, 0);
	double *ghost_top_right = lbm_mesh_get_cell(mesh, comm->width - 1, 0);
	double *ghost_bottom_left = lbm_mesh_get_cell(mesh, 0, comm->height - 1);
	double *ghost_bottom_right = lbm_mesh_get_cell(mesh, comm->width - 1, 
												   comm->height - 1);
	// ======== EXCHANGE ========

	const size_t VSIZE = (comm->height) * DIRECTIONS;
	const size_t HSIZE = (comm->width);

	// -------- SEND --------

	// Left + corners
	if (comm->rank_x - 1 >= 0) {
		MPI_Send(border_l, VSIZE, MPI_DOUBLE, 
			get_rank(comm, comm->rank_x - 1, comm->rank_y), 
			0, MPI_COMM_WORLD);

		// Top-left corner
		if (comm->rank_y - 1 >= 0) {
			MPI_Send(corner_top_left, DIRECTIONS, MPI_DOUBLE, 
				get_rank(comm, comm->rank_x - 1, comm->rank_y - 1),
				0, MPI_COMM_WORLD);
		}

		// Bottom-left corner
		if (comm->rank_y + 1 < comm->nb_y) {
			MPI_Send(corner_bottom_left, DIRECTIONS, MPI_DOUBLE, 
				get_rank(comm, comm->rank_x - 1, comm->rank_y + 1),
				0, MPI_COMM_WORLD);
		}
	}

	// Right + corners
	if (comm->rank_x + 1 < comm->nb_x) {
		MPI_Send(border_r, VSIZE, MPI_DOUBLE, 
			get_rank(comm, comm->rank_x + 1, comm->rank_y),
			0, MPI_COMM_WORLD);

		// Top-right corner
		if (comm->rank_y - 1 >= 0) {
			MPI_Send(corner_top_right, DIRECTIONS, MPI_DOUBLE, 
				get_rank(comm, comm->rank_x + 1, comm->rank_y - 1),
				0, MPI_COMM_WORLD);
		}

		// Bottom-right corner
		if (comm->rank_y + 1 < comm->nb_y) {
			MPI_Send(corner_bottom_right, DIRECTIONS, MPI_DOUBLE, 
				get_rank(comm, comm->rank_x + 1, comm->rank_y + 1),
				0, MPI_COMM_WORLD);
		}
	}

	// Top
	if (comm->rank_y - 1 >= 0) {
		MPI_Send(lbm_mesh_get_cell(mesh, 0, 1), 1, comm->type, 
			get_rank(comm, comm->rank_x, comm->rank_y - 1),
			0, MPI_COMM_WORLD);
	}

	// Bottom
	if (comm->rank_y + 1 < comm->nb_y) {
		MPI_Send(lbm_mesh_get_cell(mesh, 0, comm->height - 2), 1, comm->type, 
			get_rank(comm, comm->rank_x, comm->rank_y + 1),
			0, MPI_COMM_WORLD);
	}


	// -------- RECEIVE --------

	// Top
	if (comm->rank_y - 1 >= 0) {
		MPI_Recv(lbm_mesh_get_cell(mesh, 0, 0), 1, comm->type, 
			get_rank(comm, comm->rank_x, comm->rank_y - 1),
			0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}

	// Bottom
	if (comm->rank_y + 1 < comm->nb_y) {
		MPI_Recv(lbm_mesh_get_cell(mesh, 0, comm->height - 1), 1, comm->type, 
			get_rank(comm, comm->rank_x, comm->rank_y + 1),
			0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}

	// Left + corners
	if (comm->rank_x - 1 >= 0) {
		MPI_Recv(ghost_l, VSIZE, MPI_DOUBLE, 
			get_rank(comm, comm->rank_x - 1, comm->rank_y), 
			0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		// Top-left corner
		if (comm->rank_y - 1 >= 0) {
			MPI_Recv(ghost_top_left, DIRECTIONS, MPI_DOUBLE, 
				get_rank(comm, comm->rank_x - 1, comm->rank_y - 1),
				0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}

		// Bottom-left corner
		if (comm->rank_y + 1 < comm->nb_y) {
			MPI_Recv(ghost_bottom_left, DIRECTIONS, MPI_DOUBLE, 
				get_rank(comm, comm->rank_x - 1, comm->rank_y + 1),
				0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
	}

	// Right + corners
	if (comm->rank_x + 1 < comm->nb_x) {
		MPI_Recv(ghost_r, VSIZE, MPI_DOUBLE, 
			get_rank(comm, comm->rank_x + 1, comm->rank_y),
			0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		// Top-right corner
		if (comm->rank_y - 1 >= 0) {
			MPI_Recv(ghost_top_right, DIRECTIONS, MPI_DOUBLE, 
				get_rank(comm, comm->rank_x + 1, comm->rank_y - 1),
				0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}

		// Bottom-right corner
		if (comm->rank_y + 1 < comm->nb_y) {
			MPI_Recv(ghost_bottom_right, DIRECTIONS, MPI_DOUBLE, 
				get_rank(comm, comm->rank_x + 1, comm->rank_y + 1),
				0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
	}

	

	// ==========================
}
