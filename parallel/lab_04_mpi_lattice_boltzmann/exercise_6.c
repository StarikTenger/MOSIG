/*****************************************************
    AUTHOR  : Sébastien Valat
    MAIL    : sebastien.valat@univ-grenoble-alpes.fr
    LICENSE : BSD
    YEAR    : 2021
    COURSE  : Parallel Algorithms and Programming
*****************************************************/

//////////////////////////////////////////////////////
//
// Goal: Implement 2D grid communication with non-blocking
//       messages.
//
// SUMMARY:
//     - 2D splitting along X and Y
//     - 8 neighbors communications
//     - MPI type for non contiguous cells
// NEW:
//     - Non-blocking communications
//
//////////////////////////////////////////////////////

/****************************************************/
#include "src/lbm_struct.h"
#include "src/exercises.h"
#include  <stdbool.h> 

/****************************************************/
void lbm_comm_init_ex6(lbm_comm_t * comm, int total_width, int total_height)
{
	//we use the same implementation than ex5
	lbm_comm_init_ex5(comm, total_width, total_height);
}

/****************************************************/
void lbm_comm_release_ex6(lbm_comm_t * comm)
{
	//we use the same implementation than ext 5
	lbm_comm_release_ex5(comm);
}

/****************************************************/
void lbm_comm_ghost_exchange_ex6(lbm_comm_t * comm, lbm_mesh_t * mesh)
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
	MPI_Request request_left;
	MPI_Request request_right;
	MPI_Request request_top;
	MPI_Request request_bottom;
	MPI_Request request_top_right;
	MPI_Request request_top_left;
	MPI_Request request_bottom_left;
	MPI_Request request_bottom_right;

	MPI_Request recv_left;
	MPI_Request recv_right;
	MPI_Request recv_top;
	MPI_Request recv_bottom;
	MPI_Request recv_top_right;
	MPI_Request recv_top_left;
	MPI_Request recv_bottom_left;
	MPI_Request recv_bottom_right;

	const bool nb_left = comm->rank_x - 1 >= 0;
	const bool nb_right = comm->rank_x + 1 < comm->nb_x;
	const bool nb_top = comm->rank_y - 1 >= 0;
	const bool nb_bottom = comm->rank_y + 1 < comm->nb_y;

	// -------- SEND SIDES --------

	// Left
	if (nb_left) {
		MPI_Isend(border_l, VSIZE, MPI_DOUBLE, 
			get_rank(comm, comm->rank_x - 1, comm->rank_y), 
			0, MPI_COMM_WORLD, &request_left);
	}

	// Right
	if (nb_right) {
		MPI_Isend(border_r, VSIZE, MPI_DOUBLE, 
			get_rank(comm, comm->rank_x + 1, comm->rank_y),
			0, MPI_COMM_WORLD, &request_right);
	}

	// Top
	if (nb_top) {
		MPI_Isend(lbm_mesh_get_cell(mesh, 0, 1), 1, comm->type, 
			get_rank(comm, comm->rank_x, comm->rank_y - 1),
			0, MPI_COMM_WORLD, &request_top);
	}

	// Bottom
	if (nb_bottom) {
		MPI_Isend(lbm_mesh_get_cell(mesh, 0, comm->height - 2), 1, comm->type, 
			get_rank(comm, comm->rank_x, comm->rank_y + 1),
			0, MPI_COMM_WORLD, &request_bottom);
	}


	// -------- RECEIVE SIDES --------

	// Top
	if (nb_top) {
		MPI_Irecv(lbm_mesh_get_cell(mesh, 0, 0), 1, comm->type, 
			get_rank(comm, comm->rank_x, comm->rank_y - 1),
			0, MPI_COMM_WORLD, &recv_top);
	}

	// Bottom
	if (nb_bottom) {
		MPI_Irecv(lbm_mesh_get_cell(mesh, 0, comm->height - 1), 1, comm->type, 
			get_rank(comm, comm->rank_x, comm->rank_y + 1),
			0, MPI_COMM_WORLD, &recv_bottom);
	}

	// Left
	if (nb_left) {
		MPI_Irecv(ghost_l, VSIZE, MPI_DOUBLE, 
			get_rank(comm, comm->rank_x - 1, comm->rank_y), 
			0, MPI_COMM_WORLD, &recv_left);
	}

	// Right
	if (nb_right) {
		MPI_Irecv(ghost_r, VSIZE, MPI_DOUBLE, 
			get_rank(comm, comm->rank_x + 1, comm->rank_y),
			0, MPI_COMM_WORLD, &recv_right);
	}

	// --------  SEND CORNERS --------

	// Top-left corner
	if (nb_top  && nb_left) {
		MPI_Isend(corner_top_left, DIRECTIONS, MPI_DOUBLE, 
			get_rank(comm, comm->rank_x - 1, comm->rank_y - 1),
			0, MPI_COMM_WORLD, &request_top_left);
	}

	// Bottom-left corner
	if (nb_bottom && nb_left) {
		MPI_Isend(corner_bottom_left, DIRECTIONS, MPI_DOUBLE, 
			get_rank(comm, comm->rank_x - 1, comm->rank_y + 1),
			0, MPI_COMM_WORLD, &request_bottom_left);
	}

	// Top-right corner
	if (nb_top && nb_right) {
		MPI_Isend(corner_top_right, DIRECTIONS, MPI_DOUBLE, 
			get_rank(comm, comm->rank_x + 1, comm->rank_y - 1),
			0, MPI_COMM_WORLD, &request_top_right);
	}

	// Bottom-right corner
	if (nb_bottom && nb_right) {
		MPI_Isend(corner_bottom_right, DIRECTIONS, MPI_DOUBLE, 
			get_rank(comm, comm->rank_x + 1, comm->rank_y + 1),
			0, MPI_COMM_WORLD, &request_bottom_right);
	}

	if (nb_left) MPI_Wait(&request_left, MPI_STATUS_IGNORE);
	if (nb_right) MPI_Wait(&request_right, MPI_STATUS_IGNORE);
	if (nb_top) MPI_Wait(&request_top, MPI_STATUS_IGNORE);
	if (nb_bottom) MPI_Wait(&request_bottom, MPI_STATUS_IGNORE);

	if (nb_left) MPI_Wait(&recv_left, MPI_STATUS_IGNORE);
	if (nb_right) MPI_Wait(&recv_right, MPI_STATUS_IGNORE);
	if (nb_top) MPI_Wait(&recv_top, MPI_STATUS_IGNORE);
	if (nb_bottom) MPI_Wait(&recv_bottom, MPI_STATUS_IGNORE);

	// -------- RECEIVE CORNERS --------

	// Top-left corner
	if (nb_top && nb_left) {
		MPI_Irecv(ghost_top_left, DIRECTIONS, MPI_DOUBLE, 
			get_rank(comm, comm->rank_x - 1, comm->rank_y - 1),
			0, MPI_COMM_WORLD, &recv_top_left);
	}

	// Bottom-left corner
	if (nb_bottom && nb_left) {
		MPI_Irecv(ghost_bottom_left, DIRECTIONS, MPI_DOUBLE, 
			get_rank(comm, comm->rank_x - 1, comm->rank_y + 1),
			0, MPI_COMM_WORLD, &recv_bottom_left);
	}
	
	// Top-right corner
	if (nb_top && nb_right) {
		MPI_Irecv(ghost_top_right, DIRECTIONS, MPI_DOUBLE, 
			get_rank(comm, comm->rank_x + 1, comm->rank_y - 1),
			0, MPI_COMM_WORLD, &recv_top_right);
	}

	// Bottom-right corner
	if (nb_bottom && nb_right) {
		MPI_Irecv(ghost_bottom_right, DIRECTIONS, MPI_DOUBLE, 
			get_rank(comm, comm->rank_x + 1, comm->rank_y + 1),
			0, MPI_COMM_WORLD, &recv_bottom_right);
	}

	if (nb_top && nb_right) MPI_Wait(&request_top_right, MPI_STATUS_IGNORE);
	if (nb_bottom && nb_right) MPI_Wait(&request_bottom_right, MPI_STATUS_IGNORE);
	if (nb_top && nb_left) MPI_Wait(&request_top_left, MPI_STATUS_IGNORE);
	if (nb_bottom && nb_left) MPI_Wait(&request_bottom_left, MPI_STATUS_IGNORE);

	if (nb_top && nb_right) MPI_Wait(&recv_top_right, MPI_STATUS_IGNORE);
	if (nb_bottom && nb_right) MPI_Wait(&recv_bottom_right, MPI_STATUS_IGNORE);
	if (nb_top && nb_left) MPI_Wait(&recv_top_left, MPI_STATUS_IGNORE);
	if (nb_bottom && nb_left) MPI_Wait(&recv_bottom_left, MPI_STATUS_IGNORE);

	

	// ==========================
}
