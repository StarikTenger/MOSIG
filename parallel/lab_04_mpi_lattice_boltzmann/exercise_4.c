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
//       8 neighbors using manual copy for non
//       contiguous side and blocking communications
//
// SUMMARY:
//     - 2D splitting along X and Y
//     - 8 neighbors communications
//     - Blocking communications
//     - Manual copy for non continguous cells
//
//////////////////////////////////////////////////////

/****************************************************/
#include "src/lbm_struct.h"
#include "src/exercises.h"
#include <string.h>

/****************************************************/
void lbm_comm_init_ex4(lbm_comm_t * comm, int total_width, int total_height)
{
	//
	// TODO: calculate the splitting parameters for the current task.
	//
	int rank;
	int comm_size;
	MPI_Comm_rank( MPI_COMM_WORLD, &rank);
	MPI_Comm_size( MPI_COMM_WORLD, &comm_size);

	// We assume that division is square TODO: make more flexible
	int size = 1;
	for (;size * size < comm_size; size++);
	if (size * size != comm_size) {
		printf("Initialization ERROR: wrong comm_size\n");
		return;
	}

	// TODO: calculate the number of tasks along X axis and Y axis.
	comm->nb_x = size;
	comm->nb_y = size;

	// TODO: calculate the current task position in the splitting
	comm->rank_x = rank % comm->nb_y;
	comm->rank_y = rank / comm->nb_y;

	// TODO : calculate the local sub-domain size (do not forget the 
	//        ghost cells). Use total_width & total_height as starting 
	//        point.
	comm->width = total_width / comm->nb_x + 2;
	comm->height = total_height / comm->nb_y + 2;

	// TODO : calculate the absolute position  (in cell number) in the global mesh.
	//        without accounting the ghost cells
	//        (used to setup the obstable & initial conditions).
	comm->x = comm->rank_x * (total_width / comm->nb_x);
	comm->y = comm->rank_y * (total_height / comm->nb_y);

	//OPTIONAL : if you want to avoid allocating temporary copy buffer
	//           for every step :
	//comm->buffer_recv_down, comm->buffer_recv_up, comm->buffer_send_down, comm->buffer_send_up
	comm->buffer_recv_down = (double*)malloc((comm->width) * sizeof(double) * DIRECTIONS);
	comm->buffer_recv_up = (double*)malloc((comm->width) * sizeof(double) * DIRECTIONS);
	comm->buffer_send_down = (double*)malloc((comm->width) * sizeof(double) * DIRECTIONS);
	comm->buffer_send_up = (double*)malloc((comm->width) * sizeof(double) * DIRECTIONS);

	//if debug print comm
	lbm_comm_print(comm);
}

/****************************************************/
void lbm_comm_release_ex4(lbm_comm_t * comm)
{
	free(comm->buffer_recv_down);
	free(comm->buffer_recv_up);
	free(comm->buffer_send_down);
	free(comm->buffer_send_up);
}

int get_rank(lbm_comm_t * comm, int x, int y) {
	//printf("get_rank(%d, %d) = %d\n", x ,y, y * comm->nb_x + x);
	return y * comm->nb_x + x;
}

/****************************************************/
void lbm_comm_ghost_exchange_ex4(lbm_comm_t * comm, lbm_mesh_t * mesh)
{
	//
	// TODO: Implement the 2D communication with :
	//         - blocking MPI functions
	//         - manual copy in temp buffer for non contiguous side 
	//
	// To be used:
	//    - DIRECTIONS: the number of doubles composing a cell
	//    - double[9] lbm_mesh_get_cell(mesh, x, y): function to get the address of a particular cell.
	//    - comm->width : The with of the local sub-domain (containing the ghost cells)
	//    - comm->height : The height of the local sub-domain (containing the ghost cells)
	//
	// TIP: create a function to get the target rank from x,y task coordinate. 
	// TIP: You can use MPI_PROC_NULL on borders.
	// TIP: send the corner values 2 times, with the up/down/left/write communication
	//      and with the diagonal communication in a second time, this avoid
	//      special cases for border tasks.

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

	// Copy to buffers
	for (int i = 0; i < comm->width; i++) {
		memcpy((void*)&comm->buffer_send_up[i * DIRECTIONS], 
			   (void*)lbm_mesh_get_cell(mesh, i, 1), 
			   sizeof(double) * DIRECTIONS);
		memcpy((void*)&comm->buffer_send_down[i * DIRECTIONS], 
			   (void*)lbm_mesh_get_cell(mesh, i, comm->height - 2), 
			   sizeof(double) * DIRECTIONS);
	}

	// ======== EXCHANGE ========

	const size_t VSIZE = (comm->height) * DIRECTIONS;
	const size_t HSIZE = (comm->width) * DIRECTIONS;

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
		MPI_Send(comm->buffer_send_up, HSIZE, MPI_DOUBLE, 
			get_rank(comm, comm->rank_x, comm->rank_y - 1),
			0, MPI_COMM_WORLD);
	}

	// Bottom
	if (comm->rank_y + 1 < comm->nb_y) {
		MPI_Send(comm->buffer_send_down, HSIZE, MPI_DOUBLE, 
			get_rank(comm, comm->rank_x, comm->rank_y + 1),
			0, MPI_COMM_WORLD);
	}

	//printf("all sent\n");


	// -------- RECEIVE --------

	// Top
	if (comm->rank_y - 1 >= 0) {
		MPI_Recv(comm->buffer_recv_up, HSIZE, MPI_DOUBLE, 
			get_rank(comm, comm->rank_x, comm->rank_y - 1),
			0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		for (int i = 0; i < comm->width; i++) {
			memcpy((void*)lbm_mesh_get_cell(mesh, i, 0), 
					(void*)&comm->buffer_recv_up[i * DIRECTIONS], 
					sizeof(double) * DIRECTIONS);
		}
	}

	// Bottom
	if (comm->rank_y + 1 < comm->nb_y) {
		MPI_Recv(comm->buffer_recv_down, HSIZE, MPI_DOUBLE, 
			get_rank(comm, comm->rank_x, comm->rank_y + 1),
			0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			
		for (int i = 0; i < comm->width; i++) {
			memcpy((void*)lbm_mesh_get_cell(mesh, i, comm->height - 1), 
					(void*)&comm->buffer_recv_down[i * DIRECTIONS], 
					sizeof(double) * DIRECTIONS);
		}
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


	// Copy from buffers
	

	



	//example to access cell
	//double * cell = lbm_mesh_get_cell(mesh, local_x, local_y);
	//double * cell = lbm_mesh_get_cell(mesh, comm->width - 1, 0);

	//TODO:
	//   - implement left/write communications
	//   - implement top/bottom communication (non contiguous)
	//   - implement diagonal communications
}
