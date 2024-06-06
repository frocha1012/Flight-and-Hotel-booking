/**
 * @file main.c
 * @brief Main application file for the Travel Reservation System.
 *
 * This file contains the main function and high-level management of the application workflow.
 * It includes functions for user authentication, menu management, and session control.
 *
 * @author Fernando Rocha
 * @date 03/06/2024
 * @version 1.0
 * @link https://github.com/frocha1012/Flight-and-Hotel-booking Visit my GitHub for more projects
 *
 * Copyright (C) 2024 Fernando Rocha
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

//////////////////////////////////////////////// INCLUDES ////////////////////////////////////////////////////////////////////////
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//////////////////////////////////////////////// STRUCTS ////////////////////////////////////////////////////////////////////////

// Structs
typedef struct User {
    char username[50];
    char password[50];
    int isAdmin;
    struct User *next;  // Pointer to the next user in the list
} User;

typedef struct Flight {
    int flightNumber;
    char origin[50];
    char destination[50];
    char departureTime[20];
    char arrivalTime[20];
    int seatsAvailable;
    struct Flight *next;
} Flight;

typedef struct Hotel {
    int hotelID;
    char name[50];
    char location[100];
    int roomsAvailable;
    struct Hotel *next;
} Hotel;

typedef struct Reservation {
    int reservationID;
    char username[50]; // Linking to the user who made the reservation
    int flightNumber; // For flight reservations; -1 if not applicable
    int hotelID; // For hotel reservations; -1 if not applicable
    char status[30]; // "Pending", "Approved", "Rejected", "Cancelled", "Cancel Requested"
    struct Reservation *next;
} Reservation;

/////////////////////////////////////////////////// GLOBAL VARIABLES  /////////////////////////////////////////////////////////////////////


User *head = NULL;
Flight *flightsHead = NULL;
Hotel *hotelsHead = NULL;
Reservation *reservationsHead = NULL;

char currentUser[50] = {0};


/////////////////////////////////////////////////// DECLARATIONS /////////////////////////////////////////////////////////////////////


// Menu and user interface functions
void mainMenu();
void adminMenu();
void userMenu();

// Business logic for application features
void listUsersWithID();
void deleteUser();
void manageUsers();

// User handling functions
void registerUser();
int loginUser();
void logout();
void recommendRandomFlight();
void displayAdminNotifications();

// Data persistence functions
void loadUsers();
void saveUsers();
void saveFlightsToFile();
void loadFlightsFromFile();
void saveHotelsToFile();
void loadHotelsFromFile();
void saveReservationsToFile();
void loadReservationsFromFile();


// Utility functions
void clearInputBuffer();
void printAllUsersInMemory();
void printAllUsersWithPasswords();


// Flight Handling
void manageFlights();
void addFlight();
void deleteFlight();
void editFlight();
void listFlights();
bool flightExists(int flightNumber);

// Hotel Handling
void manageHotels();
void addHotel();
void deleteHotel();
void editHotel();
void listHotels();
bool hotelExists(int hotelID);

// Reservation Handling
void makeFlightReservation(const char *username);
void makeHotelReservation(const char *username);
void viewUserReservations(const char *username);
void handleReservationApproval(); // Admin function to approve or reject reservations
void cancelUserReservation(const char *username); // User function to request cancellation
void handleCancellationRequests(); // Admin function to handle cancellation requests
void viewAllReservations();
void saveReservationsToFile();
void loadReservationsFromFile();

// LIST RESERVAS
void listReservationsByStatus(const char *status);
void viewPendingReservations();
void viewRequestCanceledReservations();
void viewAcceptedReservations(); // NAO USADO
void viewCanceledReservations(); // NAO USADO

// AMBAS APENAS PARA O LIST DO USER VER SE HA LUGARES
void listFlightsUser();
void listHotelsUser();
int countReservationsByFlight(int flightNumber, const char* status);
int countReservationsByHotel(int hotelID, const char* status);
int calculateAvailableSeats(int flightNumber);
int calculateAvailableRooms(int hotelID);




// Declaration of functions to handle reservation IDs
int loadLastReservationID();
void saveLastReservationID(int lastID);
int generateReservationID();

void generateReservationsReport();



/////////////////////////////////////////////////// MAIN /////////////////////////////////////////////////////////////////////

int main() {
    //LOAD ALL FILES BEFORE START
    loadUsers();
    loadFlightsFromFile();
    loadHotelsFromFile();
    loadReservationsFromFile();
    loadLastReservationID();
    // LOAD DONE
    mainMenu();
    return 0;
}
/////////////////////////////////////////////////////// MENUS /////////////////////////////////////////////////////////////////

void mainMenu() {
    int choice, loginResult;
    do {
        printf("\n\nWelcome to the Travel Reservation System\n");
        printf("1. Admin Login\n");
        printf("2. User Login\n");
        printf("3. Register New Account\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1:
                loginResult = loginUser(1);  // Expecting admin login
                if (loginResult == 1) {
                    adminMenu();
                } else {
                    printf("Access denied. Incorrect credentials or not an admin.\n");
                }
                break;
            case 2:
                loginResult = loginUser(0);  // Expecting regular user login
                if (loginResult == 0) {
                    userMenu();
                } else {
                    printf("Access denied. Incorrect credentials.\n");
                }
                break;
            case 3:
                registerUser();
                break;
            case 4:
                printf("Thank you for using our system. Goodbye!\n");
                //printAllUsersInMemory(); // DEBUG (nao funcionava antes)
                saveUsers();
                saveFlightsToFile();
                saveHotelsToFile();
                saveReservationsToFile();
                saveLastReservationID(generateReservationID(1));
                exit(0);
            case 5: //hiden case DEBUG
                printAllUsersWithPasswords();
            default:
                printf("Invalid choice, please try again.\n");
        }
    } while (choice != 4);
}



void adminMenu() {
    int choice;
    do {
        displayAdminNotifications();

        printf("\n\nAdministrator Menu\n");
        printf("1. Manage Flights\n");
        printf("2. Manage Hotels\n");
        printf("3. View Reservations\n");
        printf("4. Manage Users\n");
        printf("5. Handle Reservation Approval\n");
        printf("6. Handle Cancellation Requests\n");
        printf("7. Print a Reservation Report\n");
        printf("8. Log out\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1:
                manageFlights();
                break;
            case 2:
                manageHotels();
                break;
            case 3:
                viewAllReservations();
                break;
            case 4:
                manageUsers();
                break;
            case 5:
                viewPendingReservations();
                handleReservationApproval();
                break;
            case 6:
                viewRequestCanceledReservations();
                handleCancellationRequests();
                break;
            case 7:
                generateReservationsReport();
                break;
            case 8:
                saveReservationsToFile();
                return;
            default:
                printf("Invalid choice, please try again.\n");
        }
    } while (choice != 7);
}


void userMenu() {
    int choice;
    do {
        printf("\n");
        recommendRandomFlight();
        printf("\n\nUser Menu - Currently logged as %s\n", currentUser);
        printf("1. Search Flights\n");
        printf("2. Search Hotels\n");
        printf("3. Make a Flight Reservation\n");
        printf("4. Make a Hotel Reservation\n");
        printf("5. View My Reservations\n");
        printf("6. Request Reservation Cancellation (Only Accepted Reservations can be canceled)\n");
        printf("7. Log out\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1:
                listFlights();
                break;
            case 2:
                listHotels();
                break;
            case 3:
                listFlightsUser();
                makeFlightReservation(currentUser);
                int newResID = generateReservationID(1);
                saveLastReservationID(newResID);
                break;
            case 4:
                listHotelsUser();
                makeHotelReservation(currentUser);
                newResID = generateReservationID(1);
                saveLastReservationID(newResID);
                break;
            case 5:
                viewUserReservations(currentUser);
                break;
            case 6:
                viewUserReservations(currentUser);
                cancelUserReservation(currentUser);
                break;
            case 7:
                logout();
                saveReservationsToFile();
                return;
            default:
                printf("Invalid choice, please try again.\n");
        }

    } while (choice != 7);
}

void manageUsers() {
    printf("\nUser Management\n");
    listUsersWithID();
    printf("Choose an option:\n");
    printf("1. Delete a User\n");
    printf("2. Return to Admin Menu\n");
    printf("Option: ");

    int choice;
    scanf("%d", &choice);
    clearInputBuffer();

    switch (choice) {
        case 1:
            deleteUser();
            break;
        case 2:
            return;
        default:
            printf("Invalid choice. Please try again.\n");
            break;
    }
}

void manageHotels() {
    int choice;
    do {
        printf("\nHotel Management Menu\n");
        printf("1. Add Hotel\n");
        printf("2. Delete Hotel\n");
        printf("3. Edit Hotel Details\n");
        printf("4. List All Hotels\n");
        printf("5. Return to Admin Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1:
                addHotel();
                break;
            case 2:
                deleteHotel();
                break;
            case 3:
                editHotel();
                break;
            case 4:
                listHotels();
                break;
            case 5:
                saveHotelsToFile();
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);
}

void manageFlights() {
    int choice;
    do {
        printf("\nFlight Management Menu\n");
        printf("1. Add Flight\n");
        printf("2. Delete Flight\n");
        printf("3. Edit Flight Details\n");
        printf("4. List All Flights\n");
        printf("5. Return to Admin Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1:
                addFlight();
                break;
            case 2:
                deleteFlight();
                break;
            case 3:
                editFlight();
                break;
            case 4:
                listFlights();
                break;
            case 5:
                saveFlightsToFile();
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);
}

////////////////////////////////////////// USER DELETION //////////////////////////////////////////////////////////////////////////////

void listUsersWithID() {
    User *current = head;
    int id = 1;  // Start ID from user 1
    printf("\nUsers currently registered in system:\n");
    while (current != NULL) {
        printf("ID: %d, User: %s, Admin: %s\n", id, current->username, current->isAdmin ? "Yes" : "No");
        current = current->next;
        id++;  // Increment ID for the next
    }
}

void deleteUser() {
    int id;
    printf("Enter ID of user to delete: ");
    scanf("%d", &id);
    clearInputBuffer();

    User *current = head;
    User *previous = NULL;
    int currentId = 1;

    while (current != NULL) {
        if (currentId == id) {
            if (previous == NULL) {
                head = current->next;
            } else {
                previous->next = current->next;
            }
            free(current);
            printf("User deleted successfully.\n");
            saveUsers();
            return;
        }
        previous = current;
        current = current->next;
        currentId++;
    }

    printf("User with ID %d not found.\n", id);
}
////////////////////////////////////////// HOTEL HANDLING //////////////////////////////////////////////////////////////////////////////

void addHotel() {
    int hotelID;
    printf("Enter hotel ID: ");
    scanf("%d", &hotelID);
    clearInputBuffer();

    if (hotelExists(hotelID)) {
        printf("Hotel ID %d already exists.\n", hotelID);
        return;
    }

    Hotel *newHotel = (Hotel *)malloc(sizeof(Hotel));
    if (newHotel == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    newHotel->hotelID = hotelID;

    printf("Enter hotel name: ");
    fgets(newHotel->name, sizeof(newHotel->name), stdin);
    newHotel->name[strcspn(newHotel->name, "\n")] = 0;

    printf("Enter location: ");
    fgets(newHotel->location, sizeof(newHotel->location), stdin);
    newHotel->location[strcspn(newHotel->location, "\n")] = 0;

    printf("Enter rooms available: ");
    scanf("%d", &newHotel->roomsAvailable);
    clearInputBuffer();

    newHotel->next = hotelsHead;
    hotelsHead = newHotel;
    printf("Hotel added successfully.\n");
}


bool hotelExists(int hotelID) {
    Hotel *current = hotelsHead;
    while (current != NULL) {
        if (current->hotelID == hotelID) {
            return true;
        }
        current = current->next;
    }
    return false;
}
void deleteHotel() {
    int hotelID;
    printf("Enter hotel ID to delete: ");
    scanf("%d", &hotelID);
    clearInputBuffer();

    Hotel *current = hotelsHead, *previous = NULL;
    while (current != NULL) {
        if (current->hotelID == hotelID) {
            if (previous == NULL) {
                hotelsHead = current->next;
            } else {
                previous->next = current->next;
            }
            free(current);
            printf("Hotel ID %d deleted successfully.\n", hotelID);
            return;
        }
        previous = current;
        current = current->next;
    }
    printf("Hotel ID %d not found.\n", hotelID);
}

void editHotel() {
    int hotelID;
    printf("Enter hotel ID to edit: ");
    scanf("%d", &hotelID);
    clearInputBuffer();

    Hotel *current = hotelsHead;
    while (current != NULL) {
        if (current->hotelID == hotelID) {
            printf("Editing Hotel ID: %d\n", hotelID);

            printf("Enter new hotel name: ");
            fgets(current->name, sizeof(current->name), stdin);
            current->name[strcspn(current->name, "\n")] = 0;

            printf("Enter new location: ");
            fgets(current->location, sizeof(current->location), stdin);
            current->location[strcspn(current->location, "\n")] = 0;

            printf("Enter new rooms available: ");
            scanf("%d", &current->roomsAvailable);
            clearInputBuffer();

            printf("Hotel details updated successfully.\n");
            return;
        }
        current = current->next;
    }
    printf("Hotel ID %d not found.\n", hotelID);
}

void listHotels() {
    Hotel *current = hotelsHead;
    if (current == NULL) {
        printf("No hotels available.\n");
        return;
    }
    while (current != NULL) {
        printf("Hotel ID %d: %s, Location: %s, Rooms Available: %d\n",
               current->hotelID, current->name, current->location, current->roomsAvailable);
        current = current->next;
    }
}

//////////////////////////////////////////////////// FLIGHT HANDLING ////////////////////////////////////////////////////////////////////
void addFlight() {
    int flightNumber;
    printf("Enter flight number: ");
    scanf("%d", &flightNumber);
    clearInputBuffer();

    if (flightExists(flightNumber)) {
        printf("Flight number %d already exists.\n", flightNumber);
        return;
    }

    Flight *newFlight = (Flight *)malloc(sizeof(Flight));
    if (newFlight == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    newFlight->flightNumber = flightNumber;

    printf("Enter origin: ");
    fgets(newFlight->origin, sizeof(newFlight->origin), stdin);
    newFlight->origin[strcspn(newFlight->origin, "\n")] = 0;

    printf("Enter destination: ");
    fgets(newFlight->destination, sizeof(newFlight->destination), stdin);
    newFlight->destination[strcspn(newFlight->destination, "\n")] = 0;

    printf("Enter departure time: ");
    fgets(newFlight->departureTime, sizeof(newFlight->departureTime), stdin);
    newFlight->departureTime[strcspn(newFlight->departureTime, "\n")] = 0;

    printf("Enter arrival time: ");
    fgets(newFlight->arrivalTime, sizeof(newFlight->arrivalTime), stdin);
    newFlight->arrivalTime[strcspn(newFlight->arrivalTime, "\n")] = 0;

    printf("Enter seats available: ");
    scanf("%d", &newFlight->seatsAvailable);
    clearInputBuffer();

    newFlight->next = flightsHead;
    flightsHead = newFlight;
    printf("Flight added successfully.\n");
}


bool flightExists(int flightNumber) {
    Flight *current = flightsHead;
    while (current != NULL) {
        if (current->flightNumber == flightNumber) {
            return true;
        }
        current = current->next;
    }
    return false;
}

void deleteFlight() {
    int flightNumber;
    printf("Enter flight number to delete: ");
    scanf("%d", &flightNumber);
    clearInputBuffer();

    Flight *current = flightsHead, *previous = NULL;
    while (current != NULL) {
        if (current->flightNumber == flightNumber) {
            if (previous == NULL) {
                flightsHead = current->next;
            } else {
                previous->next = current->next;
            }
            free(current);
            printf("Flight %d deleted successfully.\n", flightNumber);
            return;
        }
        previous = current;
        current = current->next;
    }
    printf("Flight number %d not found.\n", flightNumber);
}

void editFlight() {
    int flightNumber;
    printf("Enter flight number to edit: ");
    scanf("%d", &flightNumber);
    clearInputBuffer();

    Flight *current = flightsHead;
    while (current != NULL) {
        if (current->flightNumber == flightNumber) {
            printf("Editing Flight Number: %d\n", flightNumber);

            printf("Enter new origin: ");
            fgets(current->origin, sizeof(current->origin), stdin);
            current->origin[strcspn(current->origin, "\n")] = 0;

            printf("Enter new destination: ");
            fgets(current->destination, sizeof(current->destination), stdin);
            current->destination[strcspn(current->destination, "\n")] = 0;

            printf("Enter new departure time: ");
            fgets(current->departureTime, sizeof(current->departureTime), stdin);
            current->departureTime[strcspn(current->departureTime, "\n")] = 0;

            printf("Enter new arrival time: ");
            fgets(current->arrivalTime, sizeof(current->arrivalTime), stdin);
            current->arrivalTime[strcspn(current->arrivalTime, "\n")] = 0;

            printf("Enter new seats available: ");
            scanf("%d", &current->seatsAvailable);
            clearInputBuffer();

            printf("Flight details updated successfully.\n");
            return;
        }
        current = current->next;
    }
    printf("Flight number %d not found.\n", flightNumber);
}


void listFlights() {
    Flight *current = flightsHead;
    if (current == NULL) {
        printf("No flights available.\n");
        return;
    }
    while (current != NULL) {
        printf("Flight %d: %s to %s, Departure: %s, Arrival: %s, Seats: %d\n",
               current->flightNumber, current->origin, current->destination,
               current->departureTime, current->arrivalTime, current->seatsAvailable);
        current = current->next;
    }
}


///////////////////////////////////////////////// REGISTER FUNCTION ADMIN OR USER ///////////////////////////////////////////////////////////////////////

void registerUser() {
    User *newUser = (User *)malloc(sizeof(User));
    if (!newUser) {
        perror("Memory allocation failed");
        return;
    }

    printf("Enter username: ");
    scanf("%49s", newUser->username);
    clearInputBuffer();

    printf("Enter password: ");
    scanf("%49s", newUser->password);
    clearInputBuffer();

    printf("Is this an admin account? (1 for Yes, 0 for No): ");
    scanf("%d", &newUser->isAdmin);
    clearInputBuffer();

    // Prevent memory leak by initializing the next pointer to NULL
    newUser->next = NULL;

    // Check if username already exists
    User *current = head, *last = NULL;
    while (current != NULL) {
        if (strcmp(current->username, newUser->username) == 0) {
            printf("This username already exists.\n");
            free(newUser);
            return;
        }
        last = current;
        current = current->next;
    }

    // Insert at the end or as the first element
    if (last == NULL) { // No users yet, add as the first user
        head = newUser;
    } else {
        last->next = newUser; // Add new user at the end of the list
    }
    saveUsers();
    printf("User registered successfully!\n");
}

///////////////////////////////////////////////// LOGIN FUNCTION ADMIN OR USER ///////////////////////////////////////////////////////////////////////
int loginUser(int expectedAdmin) {
    char username[50];
    char password[50];
    printf("Enter username: ");
    scanf("%49s", username);
    clearInputBuffer();
    printf("Enter password: ");
    scanf("%49s", password);
    clearInputBuffer();

    User *current = head;
    while (current != NULL) {
        if (strcmp(current->username, username) == 0 && strcmp(current->password, password) == 0) {
            if (current->isAdmin == expectedAdmin) {
                strcpy(currentUser, username);  //GUARDAR CURRENT USER PRAS OUTRAS FUNÇOES ESPECIFICAS( funçoes que necessitam de user especificio)
                return current->isAdmin;  // Returns 1 for admin, 0 for regular user
            } else {
                printf("Access denied. Incorrect user role.\n");
                return -1;  // Wrong type of user for the intended operation
            }
        }
        current = current->next;
    }

    printf("Invalid username or password.\n");
    return -1;
}

void logout() {
    currentUser[0] = '\0';
    printf("You have been logged out.\n");
}


//////////////////////////////////////////////// BINARY FOR USERS ////////////////////////////////////////////////////////////////////////
void loadUsers() {
    FILE *file = fopen("users.dat", "rb");
    if (file == NULL) {
        printf("No existing user file found; starting new.\n");
        return;
    }

    User *current = NULL, *temp;
    head = NULL;

    while (1) {
        temp = (User *)malloc(sizeof(User));
        if (temp == NULL) {
            perror("Failed to allocate memory");
            break;
        }
        if (fread(temp, sizeof(User) - sizeof(User*), 1, file) != 1) {
            free(temp);
            break;
        }
        temp->next = NULL;

        if (head == NULL) {
            head = temp;
            current = head;
        } else {
            current->next = temp;
            current = temp;
        }
    }
    fclose(file);
}
void saveUsers() {
    FILE *file = fopen("users.dat", "wb");
    if (file == NULL) {
        perror("Failed to open file for writing");
        return;
    }

    User *current = head;
    while (current != NULL) {
        fwrite(current, sizeof(User) - sizeof(User*), 1, file);
        current = current->next;
    }

    fclose(file);
}

void saveFlightsToFile() {
    FILE *file = fopen("flights.txt", "w");
    if (!file) {
        perror("Failed to open flights file for writing");
        return;
    }
    Flight *current = flightsHead;
    while (current != NULL) {
        fprintf(file, "%d|%s|%s|%s|%s|%d\n",
                current->flightNumber, current->origin, current->destination,
                current->departureTime, current->arrivalTime, current->seatsAvailable);
        current = current->next;
    }
    fclose(file);
}

void loadFlightsFromFile() {
    FILE *file = fopen("flights.txt", "r");
    if (!file) {
        perror("Failed to open flights file for reading");
        return;
    }
    Flight *current = NULL;
    while (!feof(file)) {
        Flight *newFlight = (Flight *)malloc(sizeof(Flight));
        if (fscanf(file, "%d|%49[^|]|%49[^|]|%19[^|]|%19[^|]|%d\n",
                   &newFlight->flightNumber, newFlight->origin, newFlight->destination,
                   newFlight->departureTime, newFlight->arrivalTime, &newFlight->seatsAvailable) == 6) {
            newFlight->next = NULL;
            if (flightsHead == NULL) {
                flightsHead = newFlight;
                current = flightsHead;
            } else {
                current->next = newFlight;
                current = newFlight;
            }
        } else {
            free(newFlight);
            break;
        }
    }
    fclose(file);
}

void saveHotelsToFile() {
    FILE *file = fopen("hotels.txt", "w");
    if (!file) {
        perror("Failed to open hotels file for writing");
        return;
    }
    Hotel *current = hotelsHead;
    while (current != NULL) {
        fprintf(file, "%d|%s|%s|%d\n",
                current->hotelID, current->name, current->location, current->roomsAvailable);
        current = current->next;
    }
    fclose(file);
}

void loadHotelsFromFile() {
    FILE *file = fopen("hotels.txt", "r");
    if (!file) {
        perror("Failed to open hotels file for reading");
        return;
    }
    Hotel *current = NULL;
    while (!feof(file)) {
        Hotel *newHotel = (Hotel *)malloc(sizeof(Hotel));
        if (fscanf(file, "%d|%49[^|]|%99[^|]|%d\n",
                   &newHotel->hotelID, newHotel->name, newHotel->location, &newHotel->roomsAvailable) == 4) {
            newHotel->next = NULL;
            if (hotelsHead == NULL) {
                hotelsHead = newHotel;
                current = hotelsHead;
            } else {
                current->next = newHotel;
                current = newHotel;
            }
        } else {
            free(newHotel);
            break;
        }
    }
    fclose(file);
}

void saveReservationsToFile() {
    FILE *file = fopen("reservations.dat", "wb");
    if (file == NULL) {
        perror("Failed to open file for writing");
        return;
    }

    Reservation *current = reservationsHead;
    while (current != NULL) {
        fwrite(current, sizeof(Reservation) - sizeof(Reservation*), 1, file);
        current = current->next;
    }

    fclose(file);
}

void loadReservationsFromFile() {
    FILE *file = fopen("reservations.dat", "rb");
    if (file == NULL) {
        printf("No reservation file found, starting new.\n");
        return;
    }

    Reservation *current = NULL, *temp;
    reservationsHead = NULL;

    while (1) {
        temp = (Reservation *)malloc(sizeof(Reservation));
        if (fread(temp, sizeof(Reservation) - sizeof(Reservation*), 1, file) != 1) {
            free(temp);
            break;
        }
        temp->next = NULL;

        if (reservationsHead == NULL) {
            reservationsHead = temp;
            current = reservationsHead;
        } else {
            current->next = temp;
            current = temp;
        }
    }

    fclose(file);
}

int loadLastReservationID() {
    FILE *file = fopen("last_id.txt", "r");
    if (file == NULL) {
        return 1000;  // COMEÇA NO 1000 ( IF U DELETE THIS FILE, U HAVE TO DELETE RESERVATION OR SET THE VALUE INSIDE TO THE OLD ID)
    }
    int lastID;
    fscanf(file, "%d", &lastID);
    fclose(file);
    return lastID;
}


void saveLastReservationID(int lastID) {
    FILE *file = fopen("last_id.txt", "w");
    fprintf(file, "%d", lastID);
    fclose(file);
}
////////////////////////////////////////////////////////// REPORT TO TXT //////////////////////////////////////////////////////////////

void generateReservationsReport() {
    FILE *file = fopen("reservations_report.txt", "w");
    if (!file) {
        perror("Failed to open file for writing");
        return;
    }

    Reservation *current = reservationsHead;
    if (current == NULL) {
        fprintf(file, "No reservations available.\n");
    } else {
        fprintf(file, "Reservations Report:\n");
        fprintf(file, "ID | User | Flight | Hotel | Status\n");
        while (current != NULL) {
            fprintf(file, "%d | %s | %d | %d | %s\n",
                    current->reservationID,
                    current->username,
                    current->flightNumber == -1 ? 0 : current->flightNumber,
                    current->hotelID == -1 ? 0 : current->hotelID,
                    current->status);
            current = current->next;
        }
    }

    fclose(file);
    printf("Reservations report generated successfully.\n");
}


////////////////////////////////////////////////////////// NOTIFICATIONS //////////////////////////////////////////////////////////////
void recommendRandomFlight() {
    if (flightsHead == NULL) {
        printf("No flights available to recommend.\n");
        return;
    }

    // Count the number of flights
    int count = 0;
    Flight *current = flightsHead;
    while (current != NULL) {
        count++;
        current = current->next;
    }

    if (count == 0) {
        printf("No flights available to recommend.\n");
        return;
    }

    // Generate a random index for flight and phrase
    srand(time(NULL)); // Seed the random number generator
    int randomIndex = rand() % count;
    int randomPhraseIndex = rand() % 3; // Assuming there are 3 phrases

    // Traverse again to find the random flight
    current = flightsHead;
    for (int i = 0; i < randomIndex; i++) {
        current = current->next;
    }

    // Array of phrases
    const char *phrases[3] = {
            "You should take a look at this flight: Flight %d from %s to %s. It's a super hot destination among our travelers!",
            "Don't miss out on Flight %d from %s to %s. It's a top choice for out travel enthusiasts!",
            "Explore the wonders of Flight %d by booking a trip from %s to %s. Adventure awaits!"
    };

    // Display the recommendation using a random phrase
    printf(phrases[randomPhraseIndex],
           current->flightNumber, current->origin, current->destination);
}

void displayAdminNotifications() {
    printf("\n--- Administrative Notifications ---\n");
    viewPendingReservations();  // Display all pending reservations
    viewRequestCanceledReservations();  // Display all cancellation requests
    printf("\n--- End of Notifications ---\n\n");
}

///////////////////////////////////////////////// RESERVATION HANDLING ///////////////////////////////////////////////////////////////////////

void makeFlightReservation(const char *username) {
    int flightNumber;
    printf("Enter flight number to reserve or type '0' to exit: ");
    scanf("%d", &flightNumber);
    clearInputBuffer();

    if (flightNumber == 0) {
        printf("Exiting reservation process.\n");
        return;
    }

    int availableSeats = calculateAvailableSeats(flightNumber);
    if (availableSeats <= 0) {
        printf("Flight fully booked or no seats available.\n");
        return;
    }

    Reservation *newReservation = (Reservation *)malloc(sizeof(Reservation));
    if (!newReservation) {
        perror("Failed to allocate memory for reservation");
        return;
    }

    newReservation->reservationID = generateReservationID();
    strcpy(newReservation->username, username);
    newReservation->flightNumber = flightNumber;
    newReservation->hotelID = -1;
    strcpy(newReservation->status, "Pending");
    newReservation->next = reservationsHead;
    reservationsHead = newReservation;

    printf("Flight reservation made successfully! Reservation ID: %d\n", newReservation->reservationID);
    saveReservationsToFile();
}


void makeHotelReservation(const char *username) {
    int hotelID;
    printf("Enter hotel ID to reserve or type '0' to exit: ");
    scanf("%d", &hotelID);
    clearInputBuffer();

    if (hotelID == 0) {
        printf("Exiting reservation process.\n");
        return;
    }

    int availableRooms = calculateAvailableRooms(hotelID);
    if (availableRooms <= 0) {
        printf("Hotel not available or fully booked.\n");
        return;
    }

    Reservation *newReservation = (Reservation *)malloc(sizeof(Reservation));
    if (!newReservation) {
        perror("Failed to allocate memory for reservation");
        return;
    }

    newReservation->reservationID = generateReservationID();
    strcpy(newReservation->username, username);
    newReservation->flightNumber = -1;
    newReservation->hotelID = hotelID;
    strcpy(newReservation->status, "Pending");
    newReservation->next = reservationsHead;
    reservationsHead = newReservation;

    printf("Hotel reservation made successfully! Reservation ID: %d\n", newReservation->reservationID);
    saveReservationsToFile();
}


void listFlightsUser() {
    Flight *current = flightsHead;
    if (current == NULL) {
        printf("No flights available.\n");
        return;
    }
    while (current != NULL) {
        int pendingReservations = countReservationsByFlight(current->flightNumber, "Pending");
        int approvedReservations = countReservationsByFlight(current->flightNumber, "Approved");
        int availableSeats = current->seatsAvailable - (pendingReservations + approvedReservations);

        if (availableSeats < 0) availableSeats = 0;  // Ensure we don't display negative numbers

        printf("Flight %d: %s to %s, Departure: %s, Arrival: %s, Seats Available: %d\n",
               current->flightNumber, current->origin, current->destination,
               current->departureTime, current->arrivalTime, availableSeats);
        current = current->next;
    }
}

int countReservationsByFlight(int flightNumber, const char* status) {
    int count = 0;
    Reservation *current = reservationsHead;
    while (current != NULL) {
        if (current->flightNumber == flightNumber && strcmp(current->status, status) == 0) {
            count++;
        }
        current = current->next;
    }
    return count;
}
void listHotelsUser() {
    Hotel *current = hotelsHead;
    if (current == NULL) {
        printf("No hotels available.\n");
        return;
    }
    while (current != NULL) {
        int pendingReservations = countReservationsByHotel(current->hotelID, "Pending");
        int approvedReservations = countReservationsByHotel(current->hotelID, "Approved");
        int availableRooms = current->roomsAvailable - (pendingReservations + approvedReservations);

        if (availableRooms < 0) availableRooms = 0;  // Prevent negative numbers

        printf("Hotel ID %d: %s, Location: %s, Rooms Available: %d\n",
               current->hotelID, current->name, current->location, availableRooms);
        current = current->next;
    }
}
int countReservationsByHotel(int hotelID, const char* status) {
    int count = 0;
    Reservation *current = reservationsHead;
    while (current != NULL) {
        if (current->hotelID == hotelID && strcmp(current->status, status) == 0) {
            count++;
        }
        current = current->next;
    }
    return count;
}

//USER VE AS PROPRIAS RESERVAS (RECEBE USER COMO PARAMETRO)
void viewUserReservations(const char *username) {
    Reservation *current = reservationsHead;
    bool found = false;
    printf("Reservations for %s:\n", username);
    while (current != NULL) {
        if (strcmp(current->username, username) == 0) {
            printf("Reservation ID: %d, Flight: %d, Hotel: %d, Status: %s\n",
                   current->reservationID, current->flightNumber, current->hotelID, current->status);
            found = true;
        }
        current = current->next;
    }
    if (!found) {
        printf("No reservations found for this user.\n");
    }
}

// USER PEDIR CANCELAMENTOS
void cancelUserReservation(const char *username) {
    int resID;
    printf("Enter reservation ID to cancel or '0' to exit: ");
    scanf("%d", &resID);
    clearInputBuffer();

    if (resID == 0) {
        printf("Exiting...\n");
        return; // Exits if user types '0'
    }

    Reservation *current = reservationsHead;
    while (current != NULL) {
        if (current->reservationID == resID && strcmp(current->username, username) == 0) {
            if (strcmp(current->status, "Approved") == 0) {
                strcpy(current->status, "Cancel Requested");
                printf("Cancellation request submitted.\n");
                saveReservationsToFile();
                return;
            } else {
                printf("Only approved reservations can be cancelled.\n");
                return;
            }
        }
        current = current->next;
    }
    printf("Reservation not found.\n");
}

// ADMIN ACEITAR RESERVAS
void handleReservationApproval() {
    int resID;
    char decision[10];
    printf("Enter reservation ID to approve or reject, or '0' to exit: ");
    scanf("%d", &resID);
    clearInputBuffer();

    if (resID == 0) {
        printf("Exiting...\n");
        return; // Exits if user types '0'
    }

    Reservation *current = reservationsHead;
    while (current != NULL && current->reservationID != resID) {
        current = current->next;
    }
    if (current == NULL) {
        printf("Reservation not found.\n");
        return;
    }

    printf("Approve (yes) or Reject (no)? ");
    scanf("%s", decision);
    clearInputBuffer();

    if (strcmp(decision, "yes") == 0) {
        strcpy(current->status, "Approved");
        printf("Reservation approved.\n");
    } else if (strcmp(decision, "no") == 0) {
        strcpy(current->status, "Rejected");
        printf("Reservation rejected.\n");
    } else {
        printf("Invalid input.\n");
    }
    saveReservationsToFile();
}

// ADMIN ACEITAR OU NAO CANCELAMENTOS
void handleCancellationRequests() {
    int resID;
    char decision[10];

    printf("Enter reservation ID to process cancellation or '0' to exit: ");
    scanf("%d", &resID);
    clearInputBuffer();

    if (resID == 0) {
        printf("Exiting...\n");
        return;
    }

    Reservation *current = reservationsHead;
    while (current != NULL && current->reservationID != resID) {
        current = current->next;
    }

    if (current == NULL) {
        printf("Reservation not found.\n");
        return;
    }

    printf("Confirm cancellation (yes/no) or type 'exit' to leave: ");
    scanf("%9s", decision);
    clearInputBuffer();

    if (strcmp(decision, "exit") == 0) {
        printf("Exiting without making changes.\n");
        return; // Exits if user types 'exit'
    }

    if (strcmp(decision, "yes") == 0) {
        strcpy(current->status, "Cancelled");
        printf("Cancellation approved.\n");
    } else if (strcmp(decision, "no") == 0) {
        strcpy(current->status, "Approved");
        printf("Cancellation denied.\n");
    } else {
        printf("Invalid input. No changes made.\n");
    }
    saveReservationsToFile();
}

//FUNÇAO DE LISTAR SEM FILTROS
void viewAllReservations() {
    printf("\nAll Reservations:\n");
    if (reservationsHead == NULL) {
        printf("No reservations available.\n");
        return;
    }

    Reservation *current = reservationsHead;
    while (current != NULL) {
        printf("Reservation ID: %d, User: %s, ", current->reservationID, current->username);
        if (current->flightNumber != -1) {
            printf("Flight Number: %d, ", current->flightNumber);
        }
        if (current->hotelID != -1) {
            printf("Hotel ID: %d, ", current->hotelID);
        }
        printf("Status: %s\n", current->status);
        current = current->next;
    }
}

//FUNÇAO DE LISTAR MAE QUE COMPARA AO STATUS FORNECIDO
void listReservationsByStatus(const char *status) {
    Reservation *current = reservationsHead;
    int found = 0;
    printf("\nReservations with status '%s':\n", status);
    while (current != NULL) {
        if (strcmp(current->status, status) == 0) {
            printf("Reservation ID: %d, User: %s, Flight Number: %d, Hotel ID: %d\n",
                   current->reservationID, current->username,
                   current->flightNumber, current->hotelID);
            found = 1;
        }
        current = current->next;
    }
    if (!found) {
        printf("No reservations found with status '%s'.\n", status);
    }
}
//LISTAR PENDING
void viewPendingReservations() {
    listReservationsByStatus("Pending");
}
//LISTAR PEDIDOS DE CANCELAMENTO
void viewRequestCanceledReservations() {
    listReservationsByStatus("Cancel Requested");
}
//LISTAR ACEITES
void viewAcceptedReservations() {
    listReservationsByStatus("Accepted");
} // se necessario

//LISTAR CANCELADOS
void viewCanceledReservations() {
    listReservationsByStatus("Canceled");
} // se necessario

//LISTAR REJEITADOS
void viewrRejectedReservations() {
    listReservationsByStatus("Rejected");
} // se necessario
////////////////////////////////////////////////////////// NO OVERBOOKING //////////////////////////////////////////////////////////////

int calculateAvailableSeats(int flightNumber) {
    int approved = countReservationsByFlight(flightNumber, "Approved");
    Flight *flight = flightsHead;
    while (flight && flight->flightNumber != flightNumber) {
        flight = flight->next;
    }
    return (flight ? flight->seatsAvailable - approved : 0);
}

// Helper function to calculate available rooms for hotels
int calculateAvailableRooms(int hotelID) {
    int approved = countReservationsByHotel(hotelID, "Approved");
    Hotel *hotel = hotelsHead;
    while (hotel && hotel->hotelID != hotelID) {
        hotel = hotel->next;
    }
    return (hotel ? hotel->roomsAvailable - approved : 0);
}

////////////////////////////////////////////////////////// GERAR IDS //////////////////////////////////////////////////////////////

int generateReservationID() {
    static int lastID = 0;  // Static variable to keep track of the last ID
    if (lastID == 0) {
        lastID = loadLastReservationID();  // Load the last ID when the function is called for the first time
    }
    return ++lastID;  // Increment and return the new ID
}

////////////////////////////////////////////////////////// DEBUG //////////////////////////////////////////////////////////////

// TIPO DE FLUSH MAS EM FUNÇAO
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}  // Clear up to the end of the line or end-of-file
}
//FUNÇAO PARA VER QUE USERS ESTAO EM MEMORIA (HOUVE PROBLEMAS A GUARDA LOS NO INICIO)
void printAllUsersInMemory() {
    User *current = head;
    printf("Users currently registered in system:\n");
    while (current != NULL) {
        printf("User: %s, Admin: %d\n", current->username, current->isAdmin);
        current = current->next;
    }
}
void printAllUsersWithPasswords() {
    User *current = head;
    if (current == NULL) {
        printf("No users registered in the system.\n");
        return;
    }

    printf("List of all users and their passwords (DEBUG ONLY):\n");
    while (current != NULL) {
        printf("Username: %s, Password: %s, Admin: %s\n",
               current->username, current->password, current->isAdmin ? "Yes" : "No");
        current = current->next;
    }
}


////////////////////////////////////////////////////////// GUIA DE CADA FUNÇAO //////////////////////////////////////////////////////////////
/*

void mainMenu() - Menu Inicial de Login

void adminMenu() - Menu Inicial do Admin

void userMenu() - Menu Inicial do User

void manageUsers() - Menu para apagar users

void manageHotels() - Menu para adicionar, remover, editar e listar hoteis

void manageFlights() - Menu para adicionar, remover, editar e listar voos

void listUsersWithID() - Lista todos os users para saber quais apagar com um respetivo id

void deleteUser() - Apagar users

void addHotel() - Adicionar hotel

bool hotelExists(int hotelID) - Verifica se ja existe um hotel com o mesmo id ( sendo que o incremento de id nao é automatico)

void deleteHotel() - Apagar hoteis

void editHotel() - Editar hoteis

void listHotels() - Listar hoteis

void addFlight() - Adicionar voos

bool flightExists(int flightNumber) - Verifica se ja existe um voo com o mesmo id ( sendo que o incremento de id nao é automatico)

void deleteFlight() - Apagar voos

void editFlight() - Editar voos

void listFlights() - Listar voos

void registerUser() - Criar utilizador ( username / password / 0 - user & 1 - admin )

int loginUser(int expectedAdmin) - login

void logout() - logout pra dar set do currentUser a 0

void loadUsers() - load do ficheiro users.dat (binario)                             // INFORMAÇAO SENSIVEL POR ISSO FICOU EM BINARIO

void saveUsers() - save para o ficheiro users.dat (binario)

void saveFlightsToFile() - load do ficheiro flights.txt (texto)                     //FOI UTILIZADO TXT PARA SER MAIS FACIL ADICIONAR E REMOVER VOOS MANUALMENTE NO FICHEIRO

void loadFlightsFromFile() - save para o ficheiro flights.txt (texto)

void saveHotelsToFile() - load do ficheiro hotels.txt (texto)                       //FOI UTILIZADO TXT PARA SER MAIS FACIL ADICIONAR E REMOVER HOTEIS MANUALMENTE NO FICHEIRO

void loadHotelsFromFile() - save para o ficheiro hotels.txt (texto)

void saveReservationsToFile() - load do ficheiro reservations.dat (binario)         // INFORMAÇAO SENSIVEL POR ISSO FICOU EM BINARIO

void loadReservationsFromFile() - save para o ficheiro reservations.dat (binario)

int loadLastReservationID() - load do ficheiro last_id.txt (texto)                  // GUARDAR EM QUE ID VAI COMEÇAR A PROXIMA SESSAO QUANDO FOREM EFETUADAS RESERVAS

void saveLastReservationID(int lastID) - save para o ficheiro last_id.txt (texto)

void generateReservationsReport() - Cria ficheiro com todos os dados de reservas

void recommendRandomFlight() - Recomendaçao ao fazer login user

void displayAdminNotifications() - Notificaçoes de reservas pending  ou pedidos de cancelamento para o admin saber possiveis TODO

void makeFlightReservation(const char *username) - faz a reserva

void makeHotelReservation(const char *username) - faz a reserva

void listFlightsUser() - Lista os voos ao fazer reserva

int countReservationsByFlight(int flightNumber, const char* status) - Faz a conta de quantas reservas ja ha, para mostrar um numero correto de lugares no listar acima

void listHotelsUser() - Lista os hoteis ao fazer reserva

int countReservationsByHotel(int hotelID, const char* status) - Faz a conta de quantas reservas ja ha, para mostrar um numero correto de lugares no listar acima

void viewUserReservations(const char *username) - User lista as SUAS reservas

void cancelUserReservation(const char *username) - Pedido pelo user para cancelar uma reserva de estado aproved

void handleReservationApproval() - Handling do admin para recusar ou aceitar reserva inicial

void handleCancellationRequests() - Handling do admin para recusar ou aceitar cancelamento

void viewAllReservations() - Listar todas as reservas

void listReservationsByStatus(const char *status) - Chamado pelos filtros abaixo

void viewPendingReservations() - Listar com filtro as reservas

void viewRequestCanceledReservations() - Listar com filtro as reservas

void viewAcceptedReservations() - Listar com filtro as reservas

void viewCanceledReservations() - Listar com filtro as reservas

void viewrRejectedReservations() - Listar com filtro as reservas

int calculateAvailableSeats(int flightNumber) Calcula bancos disponiveis para a funçao de fazer reserva pra ter a certeza que nao ha overbooking

int calculateAvailableRooms(int hotelID) - Calcula quartos disponiveis para a funçao de fazer reserva pra ter a certeza que nao ha overbooking

int generateReservationID() - Gera o id da reserva, começam apartir de 1000

void clearInputBuffer() - parecido ao fflush(stdin) mas melhor porque o comportamento nao varia consoante ambiente em que é utilizado

void printAllUsersInMemory() - Debug pra ver users em memoria quando criados (no inicio nao estava a gravar corretamente)

void printAllUsersWithPasswords() - Debug para consultar os utilizadores, sendo que no ficheiro estao em binario e nao da pra ler
 */