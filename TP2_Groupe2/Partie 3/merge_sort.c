#include "merge_sort.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <array_size> <num_processes>\n", argv[0]);
        exit(1);
    }
    
    int array_size = atoi(argv[1]);
    int num_processes = atoi(argv[2]);
    size_t total_size = sizeof(SharedData) + array_size * sizeof(int);

    // Creation de la memoire partagee
    fd = shm_open(SEM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, total_size);
    shared_data = mmap(NULL,total_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    // Initialisation
    shared_data->size = array_size;
    shared_data->array = (int*)(shared_data + 1);

    // Journalisation
    FILE *log_file = fopen("sorted_array.txt", "w");
    printf(log_file,"Tri avec %d nombres et %d processus:\n", array_size,num_processes);
    fclose(log_file);

    // Creation semaphore
    mutex = sem_open(SEM_NAME, O_CREAT, 0644, 1);

    /* Populate the array to test the sort */
    srand(time(NULL));
    for (int i = 0; i < array_size; i++) {
        shared_data->array[i] = rand() % MAX_NUM_SIZE;
    }
   
    gettimeofday(&start_time, NULL);

    execute_merge_sort(0, shared_data->size - 1, num_processes);

    gettimeofday(&end_time, NULL);

    // Calcul du temps d'execution en secondes
    long seconds = end_time.tv_sec - start_time.tv_sec;
    long microseconds = end_time.tv_usec - start_time.tv_usec;
    double elapsed = seconds + microseconds * 1e-6;

    // Journalisation
    FILE *log_file2= fopen("sorted_array.txt", "a");
    fprintf(log_file2,"Temps d'execution du tri: %f  secondes\n", elapsed);
    fclose(log_file2);

    // Liberation de la memoire
    munmap(shared_data, sizeof(total_size));
    close(fd);
    shm_unlink("/merge_sort_shm");

    return 0;
}

void write_array_into_file(int start, int end, int *array) {
    FILE *log_file = fopen("sorted_array.txt", "a");
    fprintf(log_file, "Start = %d, End = %d, sorted = [", start, end);
    for (int i = start; i <= end; i++) {
        fprintf(log_file,"%d", array[i]);
        if (i != end) fprintf(log_file,", ");
    }
    fprintf(log_file,"]\n");
    fclose(log_file);
}

void execute_merge_sort(int start, int end, int num_processes) {
    if (num_processes <= 1 || start < end ) {
        merge_sort(start, end);
        return;
    }

    int mid = start + (end - start) / 2;
    pid_t left_pid, right_pid;

    left_pid = fork();
    if (left_pid == 0) {
        // Partie de gauche
        execute_merge_sort(start, mid, num_processes / 2);
        exit(0);
    }

    right_pid = fork();
    if (right_pid == 0) {
        // Partie de droite
        execute_merge_sort(mid + 1, end, num_processes / 2);
        exit(0);
    }

    // Attente de la fin des enfants
    waitpid(left_pid, NULL, 0);
    waitpid(right_pid, NULL, 0);

    // Syncronisation avant de merge
    sem_wait(mutex);
    merge(start, mid, end);
    sem_post(mutex);
}

void merge_sort( int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort(left, mid);
        merge_sort(mid + 1, right);
        merge(left, mid, right);
        write_array_into_file(left, right, shared_data->array);
    }
}

void merge(int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = shared_data->array[left + i];
    for (j = 0; j < n2; j++)
        R[j] = shared_data->array[mid + 1 + j];

    i = 0;
    j = 0;
    k = left;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            shared_data->array[k] = L[i];
            i++;
        } else {
            shared_data->array[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        shared_data->array[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        shared_data->array[k] = R[j];
        j++;
        k++;
    }
}

void show_array(){
    printf("Sorted array: ");
    for (int i = 0; i < shared_data->size; i++) {
        printf("%d ", shared_data->array[i]);
    }
    printf("\n");
}