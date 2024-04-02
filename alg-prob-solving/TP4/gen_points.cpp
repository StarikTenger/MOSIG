/******************************************************************************
 *           GENERATION OF INPUT DATA FOR PATHFINDING ALGORITHM               * 
*******************************************************************************/

#include <cstdlib> 
#include <iostream>
#include <time.h>
#include <vector>

using namespace std; 

int random_from_range(int l, int r	) {
	return l + rand() % (r - l);
}

int main() { 
	srand(time(0));

	int n, w, h;

	cin >> n >> w >> h;
	
	cout << n << "\n";
	

	vector<vector<bool>> used(w, vector<bool>(h));

    for (int i = 0; i < n; i++) {
		int x, y;
		do {
			x = random_from_range(0, w);
			y = random_from_range(0, h);
		} while (used[x][y]);	

        cout << x << " " << y << "\n";
		used[x][y] = 1;
	}
    return 0; 
}

/* FROG ASCII ART (from internet)!!!
          ,-.___.-.
       ,-.(|)   (|),-.
       \_*._ ' '_.* _/
        /`-.`--' .-'\
   ,--./    `---'    \,--.
   \   |(  )     (  )|   /
hjw \  | ||       || |  /
`97  \ | /|\     /|\ | /
     /  \-._     _,-/  \
    //| \\  `---'  // |\\
   /,-.,-.\       /,-.,-.\
  o   o   o      o   o    o

*/