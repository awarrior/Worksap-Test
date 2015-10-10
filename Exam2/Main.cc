#include <cstdio>
#include <cstring>
#include <climits>

#define UINT unsigned int

// city node in city neighbor linkedlist
struct city_node {
	UINT city_no;
	struct city_node* next_neighbor = NULL;

	city_node(UINT no) : city_no(no) {}
};

// save the first and last neigbor city in city neighbor linkedlist
// provide add city neighbor function
struct city_center {
	city_node* first_city = NULL;
	city_node* last_city = NULL;

	void add_neighbor(UINT city_no) {
		city_node* node = new city_node(city_no);
		if (first_city == NULL) {
			first_city = node;
			last_city = node;
		}
		else {
			last_city->next_neighbor = node;
			last_city = node;
		}
	}
};

// input validation
bool input_valid(UINT& n, UINT floor, UINT ceil) {
	int cnt = scanf("%u", &n);
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

// update shortest distance memory after announcing a new festive city using breadth-first search
void update_distance(int festive_city, UINT* shortest_distance, city_center* relations, int n) {
	// return if this city is already festive
	if (shortest_distance[festive_city] == 0)
		return;

	// city search array
	UINT* search_arr = new UINT[n];
	search_arr[0] = festive_city;
	// current distance
	UINT distance = 0;
	// index waiting to search in search_arr
	UINT j_start = 0, j_end = 1;

	// search until no essential cities left
	while (j_start < j_end) {
		// new insert index in search_arr
		UINT k = j_end;

		// update shortest distance memory
		for (UINT j = j_start; j < j_end; ++j) {
			UINT city = search_arr[j];
			shortest_distance[city] = distance;

			// insert new city node if it's benefit
			for (city_node* it = relations[city].first_city; it != NULL; it = it->next_neighbor) {
				if (shortest_distance[it->city_no] > distance + 1)
					search_arr[k++] = it->city_no;
			}
		}

		// update search index
		j_start = j_end;
		j_end = k;
		// increase current distance
		++distance;
	}
}

int main() {

	// read num of cities and num of queries
	UINT n, m;
	if (!input_valid(n, 2, 100000) || !input_valid(m, 1, 100000))
		return -1;

	// read n-1 highway relations between cities
	city_center* relations = new city_center[n + 1];
	for (UINT i = 1; i < n; ++i) {
		UINT a, b;
		if (!input_valid(a, 1, n) || !input_valid(b, 1, n))
			return -1;
		relations[a].add_neighbor(b);
		relations[b].add_neighbor(a);
	}

	// shortest distance memory to closest festive city
	UINT* shortest_distance = new UINT[n + 1];
	memset(shortest_distance, UINT_MAX, (n + 1) * sizeof(UINT));

	// start from no.1 festive city
	update_distance(1, shortest_distance, relations, n);

	// following queries
	for (UINT i = 0; i < m; ++i) {
		// read query type and city no
		UINT qi, ci;
		if (!input_valid(qi, 1, 2) || !input_valid(ci, 1, n))
			return -1;

		if (qi == 1) {
			// announce a new festive city
			update_distance(ci, shortest_distance, relations, n);
		}
		else if (qi == 2) {
			// print the shortest distance from ci
			printf("%u\n", shortest_distance[ci]);
		}
	}

	return 0;
}