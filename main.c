#include <stdio.h>
#include <stdlib.h>

#define MOVIES_LEN 5
#define MINIMAL_MOVIE_ID 2
#define WANTED_MOVIE_ID 3

typedef struct {
	int id;
	char* title;
} Movie;

typedef struct {
	Movie** movies;
	int len;
} MovieLen;

MovieLen* getMovies();

Movie* findById(int id, MovieLen* movies);

MovieLen* add(MovieLen* movies, Movie* movie);

int predicate(Movie* movie);

MovieLen* addIf(MovieLen* movies, Movie* movie, int (* predicate)(Movie*), MovieLen* (* add)(MovieLen*, Movie*));

void freeMovies(MovieLen* movies);

int main() {
	MovieLen* fetchedMovies = getMovies();
	MovieLen* queriedMovies = getMovies();

	for (int i = 0; i < fetchedMovies->len; ++i) {
		printf("%d\n", fetchedMovies->movies[i]->id);
	}

	Movie* movieToAdd = findById(WANTED_MOVIE_ID, queriedMovies);
	MovieLen* result = addIf(fetchedMovies, movieToAdd, &predicate, &add);

	printf("\n");

	for (int i = 0; i < result->len; i++) {
		printf("%d\n", result->movies[i]->id);
	}

	freeMovies(fetchedMovies);
	freeMovies(queriedMovies);

	return 0;
}

MovieLen* getMovies() {
	Movie** movies = (Movie**) malloc(MOVIES_LEN * sizeof(Movie*));
	for (int i = 0; i < MOVIES_LEN; i++) {
		movies[i] = (Movie*) malloc(sizeof(Movie*));
	}

	for (int i = 0; i < MOVIES_LEN; i++) {
		movies[i]->id = i;
		movies[i]->title = "movie";
	}

	MovieLen* movieLen = (MovieLen*) malloc(sizeof(MovieLen));
	movieLen->len = MOVIES_LEN;
	movieLen->movies = movies;

	return movieLen;
}

Movie* findById(int id, MovieLen* movies) {
	for (int i = 0; i < movies->len; i++) {
		if (movies->movies[i]->id == id) {
			return movies->movies[i];
		}
	}

	return NULL;
}

int predicate(Movie* movie) {
	return movie->id > MINIMAL_MOVIE_ID;
}

MovieLen* add(MovieLen* movies, Movie* movie) {
	if (movie == NULL) return movies;

	movies->movies = (Movie**) realloc(movies->movies, ++movies->len * sizeof(Movie*));
	movies->movies[movies->len] = movie;
	movies->movies[movies->len - 1] = movie;

	return movies;
}

MovieLen* addIf(MovieLen* movies, Movie* movie, int (* predicate)(Movie*), MovieLen* (* add)(MovieLen*, Movie*)) {
	if (predicate(movie)) {
		MovieLen* result = add(movies, movie);
		return result;
	}

	return add(movies, NULL);
}

void freeMovies(MovieLen* movies) {
	for (int i = 0; i < movies->len; i++) {
		free(movies->movies[i]);
	}

	free(movies->movies);
	free(movies);
}