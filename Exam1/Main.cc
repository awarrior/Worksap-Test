#include <cstdio>
#include <cstring>

#define LL long long

// input validation
bool input_valid(int& n, int floor, int ceil) {
	int cnt = scanf("%d", &n);
	if (cnt == 1) {
		// valid value
		if (n >= floor && n <= ceil)
			return true;
		else
			return input_valid(n, floor, ceil);
	}
	else if (cnt == -1) {
		// error or EOF
		return false;
	}
	// filter invalid value
	int gch;
	while ((gch = getchar()) != ' ' && gch != '\n');

	return input_valid(n, floor, ceil);
}

int main() {

	// accept num of rows and cols
	int n, m;
	if (!input_valid(n, 1, 500) || !input_valid(m, 1, 500))
		return -1;

	// initialize the values of grid
	int** grid = new int*[n + 1];
	for (int i = 0; i <= n; ++i)
		grid[i] = new int[m + 1];
	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= m; ++j) {
			if (!input_valid(grid[i][j], -1, 99999))
				return -1;
		}
	}

	// save the highest score of each block in left/right col until now
	LL* left_score_col = new LL[n + 1];
	memset(left_score_col, 0, (n + 1)*sizeof(LL));
	LL* right_score_col = new LL[n + 1];
	memset(right_score_col, -1, (n + 1)*sizeof(LL));

	// search from left to right col (j)
	for (int j = 1; j <= m; ++j) {
		// mark whether exits one reachable block at least
		bool reachable = false;
		// temp var for searching
		LL* temp_score_sav = new LL[n + 1];
		memset(temp_score_sav, -1, (n + 1)*sizeof(LL));
		int prev_i = 0, ceil = 0, floor = 0;

		// search from top to bottom row (i)
		for (int i = 1; i <= n; ++i) {
			// check whether left/right block is blocked
			if (left_score_col[i] == -1 || grid[i][j] == -1)
				continue;
			else
				reachable = true;

			// never searched before
			if (temp_score_sav[i] == -1) {
				// save score
				temp_score_sav[i] = grid[i][j];
				right_score_col[i] = grid[i][j] + left_score_col[i];
				prev_i = i;

				// search up
				int k = i - 1;
				for (; k > 0 && grid[k][j] != -1; --k) {
					temp_score_sav[k] = temp_score_sav[k + 1] + grid[k][j];
					right_score_col[k] = right_score_col[k + 1] + grid[k][j];
				}
				floor = k;
				// search down
				k = i + 1;
				for (; k <= n && grid[k][j] != -1; ++k) {
					temp_score_sav[k] = temp_score_sav[k - 1] + grid[k][j];
					right_score_col[k] = right_score_col[k - 1] + grid[k][j];
				}
				ceil = k;
			}
			else {
				// searched before

				// search up from prev_i
				LL add_tmp = temp_score_sav[i] - temp_score_sav[prev_i] + left_score_col[i];
				for (int k = prev_i; k > floor; --k) {
					LL cmp_val = temp_score_sav[k] + add_tmp;
					right_score_col[k] = right_score_col[k] < cmp_val ? cmp_val : right_score_col[k];
				}
				// search down from i
				add_tmp = -temp_score_sav[i - 1] + left_score_col[i];
				for (int k = i; k < ceil; ++k) {
					LL cmp_val = temp_score_sav[k] + add_tmp;
					right_score_col[k] = right_score_col[k] < cmp_val ? cmp_val : right_score_col[k];
				}
				// search from prev_i to i
				add_tmp = temp_score_sav[i] + left_score_col[i];
				for (int k = prev_i + 1; k < i; ++k) {
					LL cmp_val = add_tmp - temp_score_sav[k - 1];
					right_score_col[k] = right_score_col[k] < cmp_val ? cmp_val : right_score_col[k];
				}
			}
		}

		// break if snake cannot reach the right side
		if (!reachable) {
			printf("-1\n");
			return 0;
		}

		// teleport from top to bottom
		if (right_score_col[1] != -1 && right_score_col[n] == -1 && grid[n][j] != -1) {
			// update block score
			right_score_col[n] = grid[n][j];
			for (int k = n - 1; grid[k][j] != -1; --k)
				right_score_col[k] = right_score_col[k + 1] + grid[k][j];
		}

		// teleport from bottom to top
		if (right_score_col[1] == -1 && right_score_col[n] != -1 && grid[1][j] != -1) {
			// update block score
			right_score_col[1] = grid[1][j];
			for (int k = 2; grid[k][j] != -1; ++k)
				right_score_col[k] = right_score_col[k - 1] + grid[k][j];
		}

		// set current col save as left one
		left_score_col = right_score_col;
		right_score_col = new LL[n + 1];
		memset(right_score_col, -1, (n + 1)*sizeof(LL));
	}

	// find the highest score
	LL highest_score = 0;
	for (int i = 1; i <= n; ++i)
		highest_score = highest_score < left_score_col[i] ? left_score_col[i] : highest_score;
	printf("%lld\n", highest_score);

	return 0;
}