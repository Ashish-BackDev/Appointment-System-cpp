#include <iostream> // For input/output stream operations
#include <string> // For using strings
#include <fstream> // For file handling (open, read, write)
#include <vector> // For using dynamic arrays (vectors)

using namespace std;

// Function to clear the screen, depending on the OS
void clearScreen() {
    // Cross-platform screen clear using conditional preprocessor directives
    #ifdef _WIN32
        system("cls"); // Windows: system("cls") will clear the console
    #else
        system("clear"); // Linux/Mac: system("clear") will clear the console
    #endif
}

// Function to handle the appointment booking process
int bookAppointment() {
    clearScreen();  // Clear the screen for a clean output
    cout << "\n ----- Book Your Appointment ---- \n";
    cout << "\n ----- Available slots ---- \n";

    ifstream read("appointment.dat"); // Object of 'ifstream' to open and read the 'appointment.dat' file
    int arr[13] = {0}; // Array to track available slots (13 slots, A to M)
    bool recordFound = false; // Flag to check if appointments are found in the file

    if (read) { // If the file is opened successfully
        string line;  // Declare a variable to hold each line of the file
        while (getline(read, line)) {  // Read the file line by line
            int index = line[0] - 'A'; // Get the index based on the first character (A = 0, B = 1, ..., M = 12)
            if (index >= 0 && index < 13) { // Ensure the index is within the valid range (0-12)
                arr[index] = 1;  // Mark the slot as booked (1 means booked)
                recordFound = true; // Set flag to true as we found a record
            }
        }
        read.close();  // Close the file after reading
    }

    cout << "\n Appointment Summary by hours:";
    for (int i = 0, hour = 9; i < 13; ++i, ++hour) {  // Loop through each slot (9 AM to 9 PM)
        cout << "\n " << static_cast<char>('A' + i) << " -> " // Display the slot letter (A, B, C, ...)
             << hour << ":00 - "  // Display the hour (9, 10, 11, ...)
             << (arr[i] == 0 ? "Available" : "Booked");  // Check availability: if 0, "Available", otherwise "Booked"
    }

    char choice; // Variable to store the user's choice (A-M)
    cout << "\n\n Input your choice (A-M): "; // Prompt user for input
    cin >> choice;  // Store user input
    choice = toupper(choice);  // Convert input to uppercase for consistency

    if (choice < 'A' || choice > 'M') { // Check if the choice is invalid (outside A-M range)
        cout << "\n Error: Invalid Selection. Choose between A-M (9 AM to 9 PM)."; // Display error message
        cout << "\n Press Enter to try again..."; 
        cin.ignore(); // Ignore any extra input in the buffer
        cin.get(); // Wait for the user to press Enter
        return bookAppointment(); // Call the function again to let the user try again
    }

    int index = choice - 'A'; // Convert the choice (A-M) to index (0-12)
    if (arr[index] == 1) { // If the slot is already booked (arr[index] = 1)
        cout << "\n Error: Appointment already booked for this hour. Choose another time."; // Display error message
        cout << "\n Press Enter to try again...";
        cin.ignore();  // Ignore leftover input
        cin.get();  // Wait for user to press Enter
        return bookAppointment();  // Call the function again to let the user try again
    }

    string name; // Variable to store the user's full name
    cout << "\n Enter your full name: "; // Prompt the user for their name
    cin.ignore();  // Ignore any extra characters left in the input buffer
    getline(cin, name);  // Read the full name including spaces

    // Open the file in append mode to add the new appointment at the end without overwriting existing ones
    ofstream out("appointment.dat", ios::app);  // 'ofstream' object for writing to the file
    if (out) {  // If the file is successfully opened
        out << choice << ":" << name << "\n";  // Write the appointment details (slot + name)
        out.close();  // Close the file
        cout << "\n Appointment booked successfully for " << (index + 9) << ":00!";  // Confirm booking
    } else {
        cout << "\n Error: Could not save appointment.";  // Error message if file can't be opened
    }

    cout << "\n Press Enter to continue...";
    cin.get();  // Wait for user input before continuing
    return 0;  // Return to the calling function
}

// Function to handle appointment cancellation
int cancelAppointment() {
    clearScreen();  // Clear the screen for a clean output
    cout << "\n ----- Cancel Your Appointment ---- \n";  // Display title for canceling appointment

    ifstream read("appointment.dat");  // 'ifstream' object to read the appointments file
    vector<string> appointments;  // 'vector' to store all appointments (dynamic array)
    string line;  // Temporary variable to store each line from the file

    while (getline(read, line)) {  // Loop to read each line from the file
        appointments.push_back(line);  // Add the line (appointment) to the vector
    }
    read.close();  // Close the file after reading all data

    if (appointments.empty()) {  // If the vector is empty (no appointments)
        cout << "\n No appointments found to cancel.";  // Inform the user
        cout << "\n Press Enter to continue...";
        cin.ignore();  // Clear input buffer
        cin.get();  // Wait for the user to press Enter
        return 0;  // Return to the main function
    }

    cout << "\n Existing Appointments: \n";  // Display existing appointments
    for (size_t i = 0; i < appointments.size(); ++i) {  // Loop through the vector to print all appointments
        cout << "\n " << appointments[i];  // Display appointment details
    }

    char choice;  // Variable to store the user's choice for canceling an appointment
    cout << "\n\n Enter the appointment slot to cancel (A-M): ";  // Ask user for input
    cin >> choice;  // Store the user's input
    choice = toupper(choice);  // Convert input to uppercase

    bool found = false;  // Flag to track whether the appointment was found
    for (size_t i = 0; i < appointments.size(); ++i) {  // Loop through the appointments vector
        if (appointments[i][0] == choice) {  // If the slot matches the user's choice
            appointments.erase(appointments.begin() + i);  // Remove the appointment from the vector
            found = true;  // Set flag to true
            break;  // Exit the loop
        }
    }

    if (!found) {  // If no appointment was found for the given slot
        cout << "\n Error: No appointment found at this slot.";  // Display error message
        cout << "\n Press Enter to try again...";
        cin.ignore();  // Ignore any extra input
        cin.get();  // Wait for Enter key to continue
        return cancelAppointment();  // Call the cancelAppointment function again to retry
    }

    ofstream out("appointment.dat", ios::trunc);  // Open the file in truncate mode to overwrite the data
    if (out) {  // If the file is opened successfully
        for (const auto& app : appointments) {  // Loop through the vector to write each appointment back to the file
            out << app << "\n";  // Write each appointment to the file
        }
        out.close();  // Close the file after writing
        cout << "\n Appointment successfully canceled.";  // Confirmation message
    } else {
        cout << "\n Error: Could not update appointment file.";  // Error message if the file can't be updated
    }

    cout << "\n Press Enter to continue...";
    cin.ignore();  // Ignore any leftover input
    cin.get();  // Wait for Enter key to continue
    return 0;  // Return to the calling function
}

// Function to view existing appointments
int existingAppointment() {
    clearScreen();  // Clear the screen for clean display
    cout << "\n ----- Appointments Summary ---- \n";  // Display title

    ifstream read("appointment.dat");  // 'ifstream' object to read the appointments file
    int arr[13] = {0};  // Array to track appointment availability for 13 slots
    bool recordFound = false;  // Flag to check if appointments are present

    if (read) {  // If file opens successfully
        string line;  // Temporary variable to hold each line from the file
        while (getline(read, line)) {  // Loop through the file
            int index = line[0] - 'A';  // Convert the slot character to index (A = 0, B = 1, ..., M = 12)
            if (index >= 0 && index < 13) {  // Ensure valid index
                arr[index] = 1;  // Mark the slot as booked
                recordFound = true;  // Set flag to true
            }
        }
        read.close();  // Close the file after reading
    }

    if (!recordFound) {  // If no appointments are found
        cout << "\n No appointments found.";  // Inform the user
    } else {
        cout << "\n Appointments Summary by hours: ";
        for (int i = 0, hour = 9; i < 13; ++i, ++hour) {  // Loop through all 13 slots
            cout << "\n " << static_cast<char>('A' + i) << " -> "
                 << hour << ":00 - " << (arr[i] == 0 ? "Available" : "Booked");  // Display slot and status
        }
    }

    cout << "\n Press Enter to continue...";
    cin.ignore();  // Ignore extra input
    cin.get();  // Wait for Enter key to continue
    return 0;  // Return to main function
}

// Main menu for the appointment system
int main() {
    int choice;  // Variable to store user choice for the menu

    do {
        clearScreen();  // Clear screen for clean menu display
        cout << "\n ----- Appointment System ---- \n";  // Display main menu title
        cout << "\n 1. Book Appointment";  // Option to book appointment
        cout << "\n 2. Cancel Appointment";  // Option to cancel appointment
        cout << "\n 3. Existing Appointment Summary";  // Option to view existing appointments
        cout << "\n 0. Exit";  // Option to exit the system
        cout << "\n\n Select your option (1-3): ";  // Prompt user to choose an option
        cin >> choice;  // Store user input

        switch (choice) {  // Perform actions based on user's choice
            case 1:
                bookAppointment();  // Call bookAppointment function
                break;
            case 2:
                cancelAppointment();  // Call cancelAppointment function
                break;
            case 3:
                existingAppointment();  // Call existingAppointment function
                break;
            case 0:
                cout << "\n Exiting...";  // Exit message
                break;
            default:
                cout << "\n Invalid choice! Please choose a valid option.";  // Error for invalid choice
        }
    } while (choice != 0);  // Continue looping until the user chooses to exit (option 0)
    return 0;  // Return 0 to indicate successful execution
}
