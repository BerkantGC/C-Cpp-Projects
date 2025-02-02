#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1000
#define NUMBER_OF_RECORDS_AT_MOVIES_1 600
#define NUMBER_OF_RECORDS_AT_MOVIES_2 600

typedef struct {
	int film_id;
	char title[255];
	char description[1023];
	unsigned int release_year;
	char rental_duration;
	float rental_rate;
	unsigned char length;
	float replacement_cost;
	char rating[10];
	char last_update[30];
} RECORD_t, *RECORD;

void print_movie(RECORD_t rec) {
	printf("ID: %d\n", rec.film_id);
	printf("TITLE: %s\n", rec.title);
	printf("DESCRIPTION: %s\n", rec.description);
	printf("RELEASE YEAR: %d\n", rec.release_year);
	printf("RENTAL DURATION: %d\n", rec.rental_duration);
	printf("RENTAL RATE: %f\n", rec.rental_rate);
	printf("REPLACEMENT COST: %f\n", rec.replacement_cost);
	printf("RATING: %s\n", rec.rating);
	printf("LAST UPDATE: %s\n", rec.last_update);
	printf("\n");
}

void print_all_movies(RECORD rec, int n) {
	for (int i = 0; i < n; i++) {
		print_movie(rec[i]);
	}
}

int searchInMovies(RECORD movies, int film_id)
{
	for (int i = 0; i < NUMBER_OF_RECORDS_AT_MOVIES_2; i++)
	{
		if(movies[i].film_id == film_id)
			return 1;
	}

	return 0;	
}

int write_intersection(FILE *file, RECORD movies1, RECORD movies2) {
	int counter = 0;

	for (int i = 0; i < NUMBER_OF_RECORDS_AT_MOVIES_1; i++)
	{
		if(searchInMovies(movies2, movies1[i].film_id) == 1)
		{
			fwrite(&movies1[i], sizeof(RECORD_t), 1, file);
			counter++;
		}
	}

	return counter;
}

int write_difference(FILE *file, RECORD movies1, RECORD movies2) {
	int i = 0 , counter = 0;

	while (i < NUMBER_OF_RECORDS_AT_MOVIES_1)
	{
		if(searchInMovies(movies2, movies1[i].film_id) == 0)
		{
			fwrite(&movies1[i], sizeof(RECORD_t), 1, file);
			counter++;
		}
		i++;
	}	

	return counter;
}

int write_union(FILE *file, RECORD movies1, RECORD movies2) {	
	int counter = 0; 

	for (int i = 0; i < NUMBER_OF_RECORDS_AT_MOVIES_1; i++)
	{
		fwrite(&movies1[i], sizeof(RECORD_t), 1, file);
		counter++;
	}
	counter += write_difference(file , movies2, movies1);

	return counter;
}

int main() {
	RECORD movies1_records = (RECORD)malloc(sizeof(RECORD_t) * NUMBER_OF_RECORDS_AT_MOVIES_1);	
	FILE *movies1_file = fopen("movies-1.txt", "rb");
	if (movies1_file == NULL) {
		printf("Cannot open the movies-1.txt file.\n");
		exit(1);
	}
	fread(movies1_records, sizeof(RECORD_t) * NUMBER_OF_RECORDS_AT_MOVIES_1, 1, movies1_file);
	fclose(movies1_file);
	
	RECORD movies2_records = (RECORD)malloc(sizeof(RECORD_t) * NUMBER_OF_RECORDS_AT_MOVIES_2);
	FILE *movies2_file = fopen("movies-2.txt", "rb");
	if (movies2_file == NULL) {
		printf("Cannot open the movies-2.txt file.\n");
		exit(2);
	}
	fread(movies2_records, sizeof(RECORD_t) * NUMBER_OF_RECORDS_AT_MOVIES_2, 1, movies2_file);
	fclose(movies2_file);
	

	// TEST FUNCTIONS
	FILE *movies_intersection = fopen("movies_intersection.txt", "wb");
	FILE *movies_union = fopen("movies_union.txt", "wb");
	FILE *movies_difference = fopen("movies_difference.txt", "wb");

	printf("Intersection count: %d \n", write_intersection(movies_intersection, movies1_records, movies2_records));
	printf("Difference count: %d \n", write_difference(movies_intersection, movies1_records, movies2_records));
	printf("Union count: %d \n", write_union(movies_intersection, movies1_records, movies2_records));

	fclose(movies_intersection);
	fclose(movies_union);
	fclose(movies_difference);
	
	free(movies1_records);
	free(movies2_records);
	
	return 0;
}