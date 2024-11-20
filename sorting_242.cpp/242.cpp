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
    int n = arr.size();
    for (int i = 1; i < n; i++) {
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
    int n = arr.size();
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j;
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
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int left = 0, right = i - 1;

        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (++comparison_count, arr[mid] <= key)
                left = mid + 1;
            else
                right = mid - 1;
        }

        for (int j = i - 1; j >= left; j--) {
            arr[j + 1] = arr[j];
        }
        arr[left] = key;
    }
}

// Function to generate data based on data type; fixing the function into vector<arr> instead of int*a (the example function)
void generate_data(vector<int>& arr, int data_type) {
    int n = arr.size();
    switch (data_type) {
    case 0: // Random data
        for (int i = 0; i < n; i++) arr[i] = rand() % n;
        break;
    case 1: // Sorted data
        for (int i = 0; i < n; i++) arr[i] = i;
        break;
    case 2: // Reverse sorted data
        for (int i = 0; i < n; i++) arr[i] = n - i - 1;
        break;
    case 3: // Nearly sorted data
        for (int i = 0; i < n; i++) arr[i] = i;
        for (int i = 0; i < 10; i++) swap(arr[rand() % n], arr[rand() % n]);
        break;
    default:
        cerr << "Invalid data type!" << endl;
    }
}

// Sort function dispatcher
void sort_dispatcher(const string& algorithm, vector<int>& arr) {
    if (algorithm == "insertion_sort") {
        insertion_sort(arr);
    }
    else if (algorithm == "shell_sort") {
        shell_sort(arr);
    }
    else if (algorithm == "binary_insertion_sort") {
        binary_insertion_sort(arr);
    }
    else {
        cerr << "Unknown algorithm: " << algorithm << endl;
    }
}

// Command 1: Run sorting algorithm on input file data
void command_1(const string& algorithm, const string& input_file, const string& output_file) {
    ifstream infile(input_file);
    ofstream outfile(output_file);
    if (!infile.is_open() || !outfile.is_open()) {
        cerr << "Error opening file!" << endl;
        return;
    }

    int size;
    infile >> size;
    vector<int> arr(size);
    for (int i = 0; i < size; i++) infile >> arr[i];

    auto start = high_resolution_clock::now();
    sort_dispatcher(algorithm, arr);
    auto end = high_resolution_clock::now();

    long long duration = duration_cast<milliseconds>(end - start).count();
    outfile << size << endl;
    for (const auto& num : arr) outfile << num << " ";
    outfile.close();

    cout << "Algorithm: " << algorithm << "\nRunning time: " << duration << " ms\nComparisons: " << comparison_count << endl;
}

// Command 2: Run sorting algorithm on generated data
void command_2(const string& algorithm, int size, const string& data_order) {
    vector<int> arr(size);
    int data_type = (data_order == "-rand") ? 0 : (data_order == "-sorted") ? 1 : (data_order == "-rev") ? 2 : 3;

    generate_data(arr, data_type);
    auto start = high_resolution_clock::now();
    sort_dispatcher(algorithm, arr);
    auto end = high_resolution_clock::now();

    long long duration = duration_cast<milliseconds>(end - start).count();
    ofstream outfile("output.txt");
    outfile << size << endl;
    for (const auto& num : arr) outfile << num << " ";
    outfile.close();

    cout << "Algorithm: " << algorithm << "\nData Order: " << data_order << "\nRunning time: " << duration << " ms\nComparisons: " << comparison_count << endl;
}

// Command 3: Run sorting algorithm on all data orders
void command_3(const string& algorithm, int size) {
    const string data_orders[] = { "random", "sorted", "reverse", "nearly sorted" };
    for (int data_type = 0; data_type < 4; data_type++) {
        vector<int> arr(size);
        generate_data(arr, data_type);

        auto start = high_resolution_clock::now();
        sort_dispatcher(algorithm, arr);
        auto end = high_resolution_clock::now();

        long long duration = duration_cast<milliseconds>(end - start).count();
        cout << "Algorithm: " << algorithm << "\nData Order: " << data_orders[data_type] << "\nRunning time: " << duration << " ms\nComparisons: " << comparison_count << endl;
    }
}

// Command 4: Compare two algorithms on input file data
void command_4(const string& algorithm1, const string& algorithm2, const string& input_file) {
    ifstream infile(input_file);
    if (!infile.is_open()) {
        cerr << "Error opening file!" << endl;
        return;
    }

    int size;
    infile >> size;
    vector<int> arr1(size), arr2(size);
    for (int i = 0; i < size; i++) infile >> arr1[i];
    arr2 = arr1;

    // Run first algorithm
    auto start1 = high_resolution_clock::now();
    sort_dispatcher(algorithm1, arr1);
    auto end1 = high_resolution_clock::now();

    // Run second algorithm
    auto start2 = high_resolution_clock::now();
    sort_dispatcher(algorithm2, arr2);
    auto end2 = high_resolution_clock::now();

    cout << "Comparison:\n";
    cout << "Algorithm 1: " << algorithm1 << "\nRunning time: " << duration_cast<milliseconds>(end1 - start1).count() << " ms\nComparisons: " << comparison_count << endl;

    cout << "Algorithm 2: " << algorithm2 << "\nRunning time: " << duration_cast<milliseconds>(end2 - start2).count() << " ms\nComparisons: " << comparison_count << endl;
}

// Command 5: Compare two algorithms on generated data
void command_5(const string& algorithm1, const string& algorithm2, int size, const string& data_order) {
    vector<int> arr(size);
    int data_type = (data_order == "-rand") ? 0 : (data_order == "-sorted") ? 1 : (data_order == "-rev") ? 2 : 3;
    generate_data(arr, data_type);

    vector<int> arr1 = arr, arr2 = arr;

    // Run first algorithm
    auto start1 = high_resolution_clock::now();
    sort_dispatcher(algorithm1, arr1);
    auto end1 = high_resolution_clock::now();

    // Run second algorithm
    auto start2 = high_resolution_clock::now();
    sort_dispatcher(algorithm2, arr2);
    auto end2 = high_resolution_clock::now();

    cout << "Comparison:\n";
    cout << "Algorithm 1: " << algorithm1 << "\nRunning time: " << duration_cast<milliseconds>(end1 - start1).count() << " ms\nComparisons: " << comparison_count << endl;

    cout << "Algorithm 2: " << algorithm2 << "\nRunning time: " << duration_cast<milliseconds>(end2 - start2).count() << " ms\nComparisons: " << comparison_count << endl;
}

// Algorithm Mode: Handles commands related to a single algorithm
void algorithm_mode(int argc, char* argv[]) {
    if (argc < 5) {
        cerr << "Error: Insufficient arguments for algorithm mode.\n";
        return;
    }

    string algorithm = argv[2];
    string option = argv[4];

    if (option == "-rand" || option == "-sorted" || option == "-rev" || option == "-nsorted") {
        // Command 2: Sorting on generated data
        int size = stoi(argv[3]);
        command_2(algorithm, size, option);
    }
    else if (isdigit(option[0])) {
        // Command 3: Sorting on all data orders of given size
        int size = stoi(option);
        command_3(algorithm, size);
    }
    else {
        cerr << "Error: Invalid option for algorithm mode.\n";
    }
}

// Comparison Mode: Handles commands comparing two algorithms
void comparison_mode(int argc, char* argv[]) {
    if (argc < 5) {
        cerr << "Error: Insufficient arguments for comparison mode.\n";
        return;
    }

    string algorithm1 = argv[2];
    string algorithm2 = argv[3];
    string option = argv[4];

    if (option == "-rand" || option == "-sorted" || option == "-rev" || option == "-nsorted") {
        // Command 5: Compare on generated data
        int size = stoi(argv[5]);
        command_5(algorithm1, algorithm2, size, option);
    }
    else if (option.find(".txt") != string::npos) {
        // Command 4: Compare on data from a file
        command_4(algorithm1, algorithm2, option);
    }
    else {
        cerr << "Error: Invalid option for comparison mode.\n";
    }
}

// Main function with mode detection
int main(int argc, char* argv[]) {
    if (argc < 4) {
        cerr << "Error: Invalid number of arguments.\n";
        return 1;
    }

    string mode = argv[1];

    if (mode == "-a") {
        // Algorithm mode
        algorithm_mode(argc, argv);
    }
    else if (mode == "-c") {
        // Comparison mode
        comparison_mode(argc, argv);
    }
    else {
        cerr << "Error: Unknown mode. Use -a for Algorithm mode or -c for Comparison mode.\n";
    }

    return 0;
}