#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MOVIES 1000
#define MAX_USERS 1000
#define MAX_GENRES 50
#define MAX_TITLE_LENGTH 100

typedef struct {
    int movie_id;
    char title[MAX_TITLE_LENGTH];
    char genre[MAX_GENRES];
} Movie;

typedef struct {
    int user_id;
    int movie_id;
    int rating;
} Rating;

Movie movies[MAX_MOVIES];
Rating ratings[MAX_USERS];
int movie_count = 0;
int rating_count = 0;

void load_movies(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    while (fscanf(file, "%d,%99[^,],%49[^\n]", &movies[movie_count].movie_id, movies[movie_count].title, movies[movie_count].genre) != EOF) {
        movie_count++;
    }

    fclose(file);
}

void load_ratings(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    while (fscanf(file, "%d,%d,%d", &ratings[rating_count].user_id, &ratings[rating_count].movie_id, &ratings[rating_count].rating) != EOF) {
        rating_count++;
    }

    fclose(file);
}

void generate_movies_csv(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    fprintf(file, "1,The Shawshank Redemption,Drama\n");
    fprintf(file, "2,The Godfather,Crime\n");
    fprintf(file, "3,Pulp Fiction,Drama\n");
    // Add more movies as needed

    fclose(file);
}

void generate_ratings_csv(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    fprintf(file, "1,1,5\n");
    fprintf(file, "1,2,4\n");
    fprintf(file, "2,1,4\n");
    fprintf(file, "2,3,5\n");
    // Add more ratings as needed

    fclose(file);
}

void analyze_data() {
    int genre_count[MAX_GENRES] = {0};

    for (int i = 0; i < movie_count; i++) {
        // Simple genre count (assuming one genre per movie)
        genre_count[i]++;
    }

    printf("Movie Count: %d\n", movie_count);
    printf("Rating Count: %d\n", rating_count);
    printf("Genre Distribution:\n");
    for (int i = 0; i < movie_count; i++) {
        printf("%s: %d\n", movies[i].genre, genre_count[i]);
    }
}

void recommend_movies(int user_id) {
    int similar_users[MAX_USERS] = {0};
    int recommendations[MAX_MOVIES] = {0};

    for (int i = 0; i < rating_count; i++) {
        if (ratings[i].user_id == user_id) {
            for (int j = 0; j < rating_count; j++) {
                if (ratings[j].movie_id == ratings[i].movie_id && ratings[j].user_id != user_id) {
                    similar_users[ratings[j].user_id]++;
                }
            }
        }
    }

    int max_similar_user = 0;
    for (int i = 0; i < MAX_USERS; i++) {
        if (similar_users[i] > similar_users[max_similar_user]) {
            max_similar_user = i;
        }
    }

    for (int i = 0; i < rating_count; i++) {
        if (ratings[i].user_id == max_similar_user) {
            recommendations[ratings[i].movie_id]++;
        }
    }

    printf("Recommended Movies:\n");
    for (int i = 0; i < MAX_MOVIES; i++) {
        if (recommendations[i] > 0) {
            printf("%s\n", movies[i].title);
        }
    }
}

void create_user_profile(int user_id) {
    int user_movies[MAX_MOVIES] = {0};

    for (int i = 0; i < rating_count; i++) {
        if (ratings[i].user_id == user_id) {
            user_movies[ratings[i].movie_id] = ratings[i].rating;
        }
    }

    printf("User %d Profile:\n", user_id);
    for (int i = 0; i < MAX_MOVIES; i++) {
        if (user_movies[i] > 0) {
            printf("%s: %d\n", movies[i].title, user_movies[i]);
        }
    }
}

int main() {
    generate_movies_csv("movies.csv");
    generate_ratings_csv("ratings.csv");

    load_movies("movies.csv");
    load_ratings("ratings.csv");

    analyze_data();

    int user_id = 1; // Example user
    create_user_profile(user_id);
    recommend_movies(user_id);

    return 0;
}