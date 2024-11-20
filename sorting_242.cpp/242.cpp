#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>

using namespace std;

void swap(int& a, int& b)
{
    int x = a;
    a = b;
    b = x;
}

//-------------------------------------------------

// This function generates a random array
void GenerateRandomData(int* a, int n)
{
    srand((unsigned int)time(NULL));

    for (int i = 0; i < n; i++)
    {
        a[i] = rand() % n;
    }
}

// This function generates a sorted array (ascending order)
void GenerateSortedData(int* a, int n)
{
    for (int i = 0; i < n; i++)
    {
        a[i] = i;
    }
}

// This function generates a reverse-sorted array (descending order)
void GenerateReverseData(int* a, int n)
{
    for (int i = 0; i < n; i++)
    {
        a[i] = n - 1 - i;
    }
}

// This function generates a nearly-sorted array
void GenerateNearlySortedData(int* a, int n)
{
    for (int i = 0; i < n; i++)
    {
        a[i] = i;
    }

    srand((unsigned int)time(NULL));

    for (int i = 0; i < 10; i++)
    {
        int r1 = rand() % n;
        int r2 = rand() % n;
        swap(a[r1], a[r2]);
    }
}

void GenerateData(int* a, int n, int dataType)
{
    switch (dataType)
    {
    case 0:
        GenerateRandomData(a, n);
        break;
    case 1:
        GenerateSortedData(a, n);
        break;
    case 2:
        GenerateReverseData(a, n);
        break;
    case 3:
        GenerateNearlySortedData(a, n);
        break;
    default:
        printf("Error: unknown data type!\n");
    }
}
using namespace chrono;

// Global variable to count comparisons
long long comparison_count = 0;


// insertion sort
void insertion_sort(vector<int>& arr) {
    comparison_count = 0;
    for (int i = 1; i < arr.size(); i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && (++comparison_count, arr[j] > key)) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// shell Sort
void shell_sort(vector<int>& arr) {
    comparison_count = 0;
    for (size_t gap = arr.size() / 2; gap > 0; gap /= 2) {
        for (size_t i = gap; i < arr.size(); i++) {
            int temp = arr[i];
            size_t j;
            for (j = i; j >= gap && (++comparison_count, arr[j - gap] > temp); j -= gap) {
                arr[j] = arr[j - gap];
            }
            arr[j] = temp;
        }
    }
}

// binary insertion sort
void binary_insertion_sort(vector<int>& arr) {
    comparison_count = 0;
    for (size_t i = 1; i < arr.size(); i++) {
        int key = arr[i];
        size_t left = 0, right = i - 1;

        while (left <= right) {
            size_t mid = left + (right - left) / 2;
            if (++comparison_count, arr[mid] <= key)
                left = mid + 1;
            else
                right = mid - 1;
        }

        for (size_t j = i; j > left; j--) {
            arr[j] = arr[j - 1];
        }
        arr[left] = key;
    }
}

// Function to generate data based on data type
void generate_data(vector<int>& arr, int data_type) {
    size_t n = arr.size();
    switch (data_type) {
    case 0: // Random data
        for (size_t i = 0; i < n; i++) arr[i] = rand() % n;
        break;
    case 1: // Sorted data
        for (size_t i = 0; i < n; i++) arr[i] = i;
        break;
    case 2: // Reverse sorted data
        for (size_t i = 0; i < n; i++) arr[i] = n - i - 1;
        break;
    case 3: // Nearly sorted data
        for (size_t i = 0; i < n; i++) arr[i] = i;
        for (size_t i = 0; i < 10; i++) swap(arr[rand() % n], arr[rand() % n]);
        break;
    default:
        cerr << "Invalid data type!" << endl;
    }
}

// Function to log results to a file
void log_results(ofstream& file, const string& data_order, int data_size, const string& algorithm, long long comparisons, long long time) {
    file << data_order << "\t" << data_size << "\t" << algorithm << "\t" << comparisons << "\t" << time << "\n";
}

// Function to execute and measure sorting algorithms
void execute_algorithm(void (*sort_function)(vector<int>&), const string& algorithm_name, vector<int> data, const string& data_order, int data_size, ofstream& output_file) {
    cout << "Running: " << algorithm_name << " | Data order: " << data_order << " | Size: " << data_size << endl;

    auto start = high_resolution_clock::now();
    sort_function(data);
    auto end = high_resolution_clock::now();

    long long duration = duration_cast<milliseconds>(end - start).count();
    log_results(output_file, data_order, data_size, algorithm_name, comparison_count, duration);
}

// Main program
int main() {
    const int sizes[] = { 10000, 30000, 50000, 100000, 300000, 500000 };
    const string data_orders[] = { "random", "sorted", "reverse", "nearly_sorted" };

    // Open output file
    ofstream output_file("output.txt");
    if (!output_file.is_open()) {
        cerr << "Error: Could not open output.txt!" << endl;
        return 1;
    }

    // Write header
    output_file << "Data Order\tData Size\tAlgorithm\tComparisons\tRunning Time (ms)\n";

    // Execute experiments
    for (int size : sizes) {
        for (int order = 0; order < 4; order++) {
            vector<int> data(size);
            generate_data(data, order);

            execute_algorithm(insertion_sort, "insertion_sort", data, data_orders[order], size, output_file);
            execute_algorithm(shell_sort, "shell_sort", data, data_orders[order], size, output_file);
            execute_algorithm(binary_insertion_sort, "binary_insertion_sort", data, data_orders[order], size, output_file);
        }
    }

    // Close file
    output_file.close();
    cout << "Experiments completed. Results saved in output.txt." << endl;

    return 0;
}
