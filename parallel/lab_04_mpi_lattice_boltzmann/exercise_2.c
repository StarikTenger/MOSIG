/*****************************************************
    AUTHOR  : Sébastien Valat
    MAIL    : sebastien.valat@univ-grenoble-alpes.fr
    LICENSE : BSD
    YEAR    : 2021
    COURSE  : Parallel Algorithms and Programming
*****************************************************/

//////////////////////////////////////////////////////
//
// Goal: Implement odd/even 1D blocking communication scheme 
//       along X axis.
//
// SUMMARY:
//     - 1D splitting along X
//     - Blocking communications
// NEW:
//     - >>> Odd/even communication ordering <<<<
//
//////////////////////////////////////////////////////

/****************************************************/
#include "src/lbm_struct.h"
#include "src/exercises.h"

/****************************************************/
void lbm_comm_init_ex2(lbm_comm_t * comm, int total_width, int total_height)
{
	//we use the same implementation then ex1
	lbm_comm_init_ex1(comm, total_width, total_height);
}

/****************************************************/
void lbm_comm_ghost_exchange_ex2(lbm_comm_t * comm, lbm_mesh_t * mesh)
{
	double *border_l = lbm_mesh_get_cell(mesh, 1, 0);
	double *border_r = lbm_mesh_get_cell(mesh, comm->width - 2, 0);
	double *ghost_l = lbm_mesh_get_cell(mesh, 0, 0);
	double *ghost_r = lbm_mesh_get_cell(mesh, comm->width - 1, 0);

	for (int i = 0; i < 2; i++) {
		if (i == comm->rank_x % 2) {
			if (comm->rank_x - 1 >= 0) {
				MPI_Send(border_l, comm->height * DIRECTIONS, MPI_DOUBLE, 
				comm->rank_x - 1, 0, MPI_COMM_WORLD);
			}
			if (comm->rank_x + 1 < comm->nb_x) {
				MPI_Send(border_r, comm->height * DIRECTIONS, MPI_DOUBLE, 
					comm->rank_x + 1, 0, MPI_COMM_WORLD);
			}
		} else {
			if (comm->rank_x - 1 >= 0) {
				MPI_Status st;
				MPI_Recv(ghost_l, comm->height * DIRECTIONS, MPI_DOUBLE, 
					comm->rank_x - 1, 0, MPI_COMM_WORLD, &st);
			}
			if (comm->rank_x + 1 < comm->nb_x) {
				MPI_Status st;
				MPI_Recv(ghost_r, comm->height * DIRECTIONS, MPI_DOUBLE, 
					comm->rank_x + 1, 0, MPI_COMM_WORLD, &st);
			}
		}
	}
}
