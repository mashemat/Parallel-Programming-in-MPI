#include <stdio.h>
#include <string.h>
#include <mpi.h>

// Define a structure to represent a transaction
struct Transaction {
    double amount;
    char operation[20];  // Operation type: deposit or withdraw
    int account_number;
};

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // Get the rank of the process
    
    // Define a transaction struct
    struct Transaction transaction;

    if (rank == 0) {  // Customer
        // Initialize the struct with data (hardcoded for simplicity)
        transaction.amount = 500;
        snprintf(transaction.operation, sizeof(transaction.operation), "deposit");
        transaction.account_number = 123456;

        // Send the transaction to the bank server (rank 1)
        MPI_Send(&transaction, sizeof(transaction), MPI_BYTE, 1, 0, MPI_COMM_WORLD);
        printf("Customer sent transaction: %.2f, %s, Account %d\n", transaction.amount, transaction.operation, transaction.account_number);
    } 
    else if (rank == 1) {  // Bank Server
        // Receive the transaction from the customer (rank 0)
        MPI_Recv(&transaction, sizeof(transaction), MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Bank received transaction: %.2f, %s, Account %d\n", transaction.amount, transaction.operation, transaction.account_number);

        // Process the transaction here
        if (strcmp(transaction.operation, "deposit") == 0) {
            printf("Processing deposit of %.2f to account %d\n", transaction.amount, transaction.account_number);
        }
        // You could add additional conditions for other operations (withdraw, etc.)
    }

    MPI_Finalize();
    return 0;
}
