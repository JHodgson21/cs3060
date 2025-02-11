#include <bitset>
#include <iostream>

using namespace std;

int extractBits(unsigned int num, unsigned int start_bit, unsigned int end_bit) {
    // Right shift to align starting bit with LSB
    unsigned int shiftedNum = num >> start_bit;

    // Create bitmask
    unsigned int bitmask = (1u << (end_bit - start_bit + 1)) - 1;

    // Perform bitwise AND to extract bits
    unsigned int extractedBits = shiftedNum & bitmask;

    return extractedBits;
}

int main() {
    cout << "Enter the integer that holds the bits: ";
    unsigned int n;
    cin >> n;

    cout << "Enter the rightmost bit position: ";
    unsigned int k;
    cin >> k;

    cout << "Enter the leftmost bit position: ";
    unsigned int m;
    cin >> m;

    // Extract bits and print the result
    unsigned int result = extractBits(n, k, m);

    // Print the result
    cout << "Extracting bits " << k << " through " << m << " from " << n
         << " = " << bitset<32>(n) << ":\n";
    cout << result << " = " << bitset<32>(result) << endl;

    return 0;
}
