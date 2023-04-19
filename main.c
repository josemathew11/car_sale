// it requires visual studio to be working with scanf()
#define _CRT_SECURE_NO_WARNINGS

// needs to make scanf() and printf() work
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// this represents a variable that can be assigned a value directly
// it represents the number of each most recent error
extern int errno;


/* Constants that are applicable to all Functions  */

#define DISCOUNT_MIN_AGE 18
#define DISCOUNT_MAX_AGE 25
#define DISCOUNT_PERCENTAGE 0.20f // this represents 20%
#define MAX_SALES 9
#define TOTAL_NO_OF_CAR_MODELS

#define NO_OF_COLORS
#define errno (*_errno())
#define CSV_FILE "data.csv" // in the visual studio project folder, a file is created

#define MENU_OPTION_BUY_CARS 'a'
#define MENU_OPTION_VIEW_SALES_DATA 'b' // ditto
#define MENU_OPTION_VIEW_CAR_STOCK 'c' // ditto
#define MENU_OPTION_EXIT 'x' // ditto
#define TRUE 1 // ditto
#define FALSE 0 // ditto
// create a new data type called "bool" that can accept unsigned char type values
// min value: 0     max value: 255
#define bool unsigned char

/* Variables that can be used in any of the functions mentioned below */
// don't need negative values, so unsigned short is most appropriate
unsigned short carsAvailable = 20;

// it will check how many car sales took place
//ticketAmountPerSale, typeOfTicketPerSale, discountGivenPerSale, customerNames
unsigned short numberOfSales = 0;
// this array will include the total cars sold throughout every sale
unsigned short carAmountPerSale[MAX_SALES];

unsigned short typeOfCarPerSale[MAX_SALES];
// this array will contain details about each sale's discount
bool discountGivenPerSale[MAX_SALES];
// contains customer name in each sale,maximum of 200 characters
char customerNames[MAX_SALES][201];

// contains information about each type of ticket;
// this data is combined with the ticketTypes array
float carPrices[] = {3.0f,4.5f,5.5f};

// contains the car types
char carTypes[][10] = { "SEDAN", "SUV", "HATCHBACK" };

// colors that can select for the car
char carColors[NO_OF_COLORS][10] = {"WHITE","BLACK","RED","BLUE","GREEN"};
// car models which are given in the function
char sedanTypeCarModelNames[TOTAL_NO_OF_CAR_MODELS][19] = {"BMW 3 SERIES","BMW 4 SERIES","BMW 5 SERIES"};
char suvTypeCarModelNames[TOTAL_NO_OF_CAR_MODELS][19] = {"BMW X3","BMW X4","BMW X5"};
char hatchbackTypeCarModelNames[TOTAL_NO_OF_CAR_MODELS][19] = {"BMW M140I","BMW 125I","BMW 118I"};

// array including the availability of each car type
unsigned short carsAvailableForEachModel[TOTAL_NO_OF_CAR_MODELS] = {5,5,5};

// array that shows the car prices of each car
unsigned int sedanTypeCarPrices[TOTAL_NO_OF_CAR_MODELS] = {40000,52000,65000};
unsigned int suvTypeCarPrices[TOTAL_NO_OF_CAR_MODELS] = {43000,60000,72000};
unsigned int hatchbackTypeCarPrices[TOTAL_NO_OF_CAR_MODELS] = {52000,50000,48000};

// Files Variables & Constants
#define FILE_OPENED 0
#define FILE_CLOSED 1
#define FILE_ERROR 2

// will hold the file stream once opened
FILE* file;
unsigned char fileStatus = FILE_CLOSED;

/* Functions the program may use */

void clearScreen() {
    // this tells the Command Prompt (i.e. the System's output mechanism)
    // to do something to clear the screen/console
    system("cls");
}

void pauseProgram(char userChoice) {
    // give the user a chance to read the previous output, allow them to continue with menu
    // customise the output on the user's choice
    if (userChoice == MENU_OPTION_EXIT) {
        printf("\n\nPress Enter to Exit...");
    }
    else {
        printf("\n\nPress Enter to return to the Menu...");
    }

    // two of these getchar() are needed to skip the newline character
    // that's likely floating around the console window
    // however, if the userChoice value is an underscore character,
    // then this is only used when pausing after a file error occurred
    // so only one getchar() is needed
    if (userChoice != '_') {
        getchar();
    }

    getchar();
}

char getCharFromConsole(char* message) {
    char userInput;
    // prompt user using the value of the given message
    printf("%s", message);
    // capture the character
    scanf("\n%c", &userInput);
    // return the captured value
    return userInput;
}

unsigned short getUnsignedShortFromConsole(char* message) {
    // will be used to save the entered number
    unsigned short userInput;

    // prompt user using the value of the given message
    printf("%s", message);
    scanf("%hd", &userInput);

    // finally, return/export the value, so it can be used by whatever statement called this Function
    return userInput;
}

// *str will allow this function to establish a connection between this str variable
// and the one that will exist at the time/place where the function is executed, thus any direct
// modification of *str in here will actually also modify the existing string variable,
// as they will both be pointing to the same address in memory
void getStringFromConsole(char* message, char* str) {

    printf("%s", message);
    // as we're now working with the pointer str, we don't need & here
    scanf("\n%[^\n]s", str);
}

void swapUnsignedShort(unsigned short* a, unsigned short* b) {
    unsigned short temp;
    // copy the value found at the pointer a's address into the newly created temp variable
    temp = *a;
    // copy the value found at the pointer a's address into the address represented by the pointer b
    *a = *b;
    // copy the value of the temp variable to the pointer b's address
    *b = temp;
}

void swapBool(bool* a, bool* b) {
    bool temp;
    // copy the value found at the pointer a's address into the newly created temp variable
    temp = *a;
    // copy the value found at the pointer a's address into the address represented by the pointer b
    *a = *b;
    // copy the value of the temp variable to the pointer b's address
    *b = temp;
}

/* START FILES FUNCTIONS */

// returns something that is not NULL if successful
FILE* createFile(char fileName[201]) {
    // open the file for writing
    file = fopen(fileName, "w");
    // then close it immediately if successful
    if (file != NULL) {
        fclose(file);
    }
    // return the pointer so that the success may be checked
    return file;
}

// mode is one of: "w" for writing, "r" for reading, "a" for appending
// this means starting from the end of the file, will not touch existing contents
// there are other modes available, I encourage you to look these up
void openFile(char fileName[201], char mode[4]) {
    // attempt to open the file
    file = fopen(fileName, mode);

    // if file's value is NULL, then something happened...
    if (file == NULL) {

        // attempt to create it first and then check if it's still NULL, meaning
        //		that something still failed
        if (createFile(fileName) == NULL) {
            fileStatus = FILE_ERROR;

            // will produce the output: Error opening D:\data.csv: No such file of directory
            printf("Error opening %s: %s", fileName, strerror(errno));
        }
        else {
            // we managed to create the file ... so now ...
            // execute this function again with the same parameters
            //		because it should now succeed
            openFile(fileName, mode);
        }

    }
        // otherwise, all OK
    else {
        fileStatus = FILE_OPENED;
    }
}

// should be called once the operations on
// the currently opened file have concluded
void closeFile() {
    // only attempt to close the file if it's already open
    if (fileStatus == FILE_OPENED) {
        fclose(file);
        fileStatus = FILE_CLOSED;
    }
}

// will return an array of strings representing the
// customer names stored in the file
void readDataFromFile() {
    // will keep track of how many lines were read from the file
    int lineCounter = 0;

    // this is an infinite loop, we'll manually stop it once we reach the end of the file
    while (1) {

        unsigned short carAmountPerSaleValue = 0, typeOfCarPerSaleValue = 0, discountGivenPerSaleValue = 0;
        char customerNameValue[201] = "";

        int scanResult = fscanf(
                file, // the file stream
                "%hd,%hd,%hd,%[^\n]s", // the format of the line
                &carAmountPerSaleValue, // the variables, one for each placeholder in the format above
                &typeOfCarPerSaleValue,
                &discountGivenPerSaleValue,
                customerNameValue
        );

        // if we reached the end of the file
        if (scanResult == EOF) {
            // then, stop the loop
            break;
        }

        // add the bits of data that were read above into the correct arrays
        carAmountPerSale[lineCounter] = carAmountPerSaleValue;
        typeOfCarPerSale[lineCounter] = typeOfCarPerSaleValue;
        // also cast (convert) the discountGivenPerSaleValue from unsigned short to a bool type
        //		before putting it in the discountGivenPerSale array
        discountGivenPerSale[lineCounter] = (bool)discountGivenPerSaleValue;
        // need to use strcpy here because we're working with strings
        strcpy(customerNames[lineCounter], customerNameValue);

        // increment the lineCounter, ready for next line that might be read
        lineCounter++;
    }

    // make sure the numberOfSales variable is also aware of how many sales are available after the above operation
    numberOfSales = lineCounter;
}

void getDataFromFile() {
    openFile(CSV_FILE, "r");

    if (fileStatus == FILE_OPENED) {
        readDataFromFile();
    }
    else if (fileStatus == FILE_ERROR) {
        printf("There was an error trying to read from the file %s.", CSV_FILE);
        // this function requires a char value, so we give it one
        //		that'll tell it we're using it because of a file error
        //			see the function body, it's been updated to check for
        //			this underscore character
        pauseProgram('_');
    }

    closeFile();
}

void writeDataToFile() {
    // loop through every sale
    for (int i = 0; i < numberOfSales; i++) {

        // this string will be built up bit by bit before being written to the opened file
        char line[201];
        // this string will contain the converted int value
        char data[50];

        // convert the unsigned short value into a string, put it in the data string variable;
        //		the "10" refers to base 10, which is what regular numeric values are written in
        //		e.g. saying you're 22 years old means that 22, in this case, is written in base 10
        //			because that's how we people use numbers by default
        // also cast the unsigned short value into an int type before converting
        _itoa((int)carAmountPerSale[i], data, 10);
        // add the amount of tickets to the line; first time we use strcpy,
        //		then strcat to add more to the string
        strcpy(line, data);
        // add a comma to separate this value from the next on this line
        strcat(line, ",");

        // convert the value into a string
        _itoa((int)typeOfCarPerSale[i], data, 10);
        // add the type of ticket to the line
        strcat(line, data);
        // add a comma to separate this value from the next on this line
        strcat(line, ",");

        // convert the value into a string
        _itoa((int)discountGivenPerSale[i], data, 10);
        // add the discount given to the line
        strcat(line, data);
        // add a comma to separate this value from the next on this line
        strcat(line, ",");

        // add the customer name to the line
        strcat(line, customerNames[i]);

        // write line to file
        fprintf(file, "%s", line);

        // only add a newline character if we're not yet writing the very last
        //		line in the file
        if (i < numberOfSales - 1) {
            fprintf(file, "\n");
        }
    }
}

void saveDataToFile() {
    openFile(CSV_FILE, "w");

    if (fileStatus == FILE_OPENED) {
        writeDataToFile();
    }
    else if (fileStatus == FILE_ERROR) {
        printf("There was an error trying to write to the file %s.", CSV_FILE);
        // this function requires a char value, so we give it one
        //		that'll tell it we're using it because of a file error
        //			see the function body, it's been updated to check for
        //			this underscore character
        pauseProgram('_');
    }

    closeFile();
}

/* END FILES FUNCTIONS */

void menu_greetCustomer() {
    // greet the user
    printf("Welcome to ROYAL DRIVE by BMW!\n\n");
}

void menu_showMenu() {
    // present the various actions the user can choose from
    printf("Menu:\n");
    printf("Car Sales\n");
    printf("%c. Buy Cars\n", MENU_OPTION_BUY_CARS);
    printf("%c. View Sales Data\n", MENU_OPTION_VIEW_SALES_DATA);
    printf("%c. View Car Stock \n", MENU_OPTION_VIEW_CAR_STOCK);
    printf("%c. Exit\n\n", MENU_OPTION_EXIT);
}



void menu_showCarTypes() {
    // calculate the number of ticket types by asking the ticketPrices
    // array about how many bytes it holds in memory and dividing that
    // result by the number of bytes a float data type holds in memory
    // the division result will be the number of ticket types (3 in this case)
    int numberOfCars = sizeof(carPrices) / sizeof(float);

    // show the user the types of tickets
    printf("\nCar Types:\n");

    for (int i = 0; i < numberOfCars; i++) {
        printf("%d - %s\n", i, carTypes[i]);
    }
}

float menu_applyDiscount(float currentPrice) {
    // multiply the price value by the discount percentage
    // this is: 1 - DISCOUNT_PERCENTAGE (0.20) => 0.80 (or, 80%)
    // so, we calculate the discounted value by reducing it to 80% of its value
    return currentPrice * (1 - DISCOUNT_PERCENTAGE);
}

bool menu_checkIfDiscountIsNeeded(unsigned short userAge) {
    // check if we need to give a discount
    if (userAge >= DISCOUNT_MIN_AGE && userAge <= DISCOUNT_MAX_AGE) {
        // return the TRUE value to indicate that discount is to be given
        return TRUE;
    }
    else {
        // check if they have an NUS card, then we can also apply the discount
        char BLUECardResponse = getCharFromConsole("\nDo you have an BLUE card ? Answer 'y' or 'n' : ");

        if (BLUECardResponse == 'y') {
            // return the TRUE value to indicate that discount is to be given
            return TRUE;
        }
    }

    // return the FALSE value to indicate that no discount is to be given
    return FALSE;
}

void menu_printDiscountOutcome(bool giveDiscount) {
    // inform the user about the discount outcome
    // always a good idea to separate the calculation from the output messages, makes it easier to work with the code
    switch (giveDiscount) {
        // a discount was applied
        // please note that no curly brackets { } are needed
        case TRUE:
            printf("\nYou get a discount!");
            break;
            // no discount was applied
        case FALSE:
            printf("\nNo discount given.");
            break;
    }
}

void menu_buyCars() {
    printf("Buy Car:\n\n");

    // check if we have at least 1 ticket available
    if (carsAvailable > 0) {
        printf("There are %hd cars available.\n\n", carsAvailable);


    }
    else {
        printf("Sorry, there are no more cars available.");
        // this allows us to terminate/stop the Function early
        return;
    }

    /* Variables - these are only used inside menu_buyTickets() Function and nowhere else */
    // don't need negative values; these are generally related to one sale, used when purchasing tickets
    unsigned short carsNeeded = 0, userAge = 0, carType;
    bool giveDiscount = FALSE;
    float totalPrice = 0;

    // get the "getStringFromConsole()" Function to directly store the customer's name
    // into the "customerNames" array at position "numberOfSales"
    //		=> we need the & operator to get the address of that array's position sent to the Function
    //			so that it can directly put the value at that address in memory
    getStringFromConsole("What is your name? Name: ", customerNames[numberOfSales]);



    printf("WHITE,BLACK,RED,BLUE,GREEN\n");

    printf("Which type of color do you want: ");
    char carColors;
    scanf("%s", &carColors);

    // check if we have enough tickets
    // no "else" needed here
    if (carsAvailable < carsNeeded) {
        printf("Sorry, there are fewer cars remaining than you require.");
        // we will terminate the Function here
        return;
    }

    // also record the number of tickets needed in the ticketAmountPerSale array
    carAmountPerSale[numberOfSales] = carsNeeded;

    // now, we'll determine what type of ticket the user wants; this will influence the price
    menu_showCarTypes();

    // get the value representing the ticket type from the user
    carType = getUnsignedShortFromConsole("\nWhich type of car do you need? Choose from above: ");
    // also record the type of these tickets in the typeOfTicketPerSale array
    typeOfCarPerSale[numberOfSales] = carType;

    // calculate total price for this sale by using the correct ticket type's price
    totalPrice = carsNeeded * carPrices[carType];

    // update number of tickets available by subtracting from it the amount of tickets needed
    carsAvailable -= carsNeeded;

    // ask for the user's age
    userAge = getUnsignedShortFromConsole("How old are you? Age: ");

    // calculate the value of giveDiscount by making use of this Function and the user's age
    giveDiscount = menu_checkIfDiscountIsNeeded(userAge);

    // actually apply the discount if needed
    if (giveDiscount == TRUE) {
        totalPrice = menu_applyDiscount(totalPrice);
    }

    // also record the outcome in the discountGivenPerSale array
    discountGivenPerSale[numberOfSales] = giveDiscount;

    // inform the user about the discount outcome
    menu_printDiscountOutcome(giveDiscount);


// present the outcome
    printf("\n\nThank you.\n");
    printf("You have bought %hd car.\n", carsNeeded);
    printf("Total cost is %f GBP.\n", totalPrice);
    printf("\nThere are %hd cars remaining.", carsAvailable);

    // finally, add 1 to the numberOfSales counter
    numberOfSales++;
}

void sortArraysByNumberOfCarsSoldPerSale() {

    // set up a loop that gives us an index "i" for accessing
    //		between the (first) and (second to last) positions that contain values
    for (int i = 0; i < numberOfSales - 1; i++) {

        // set up a loop the gives us an index "j" for accessing
        //		between the (first immediately after i) and (last) positions that contain values
        // this loop will execute fully, from start to finish,
        //		every time the above for loop begins a new iteration
        for (int j = i + 1; j < numberOfSales; j++) {

            // check if the value at position I is greater than the value at position j
            if (carAmountPerSale[i] > carAmountPerSale[j]) {

                // if so, swap those two values in the ticketAmountPerSale array
                swapUnsignedShort(&carAmountPerSale[i], &carAmountPerSale[j]);

                // also swap the two values at those same positions in the typeOfTicketPerSale array
                swapUnsignedShort(&typeOfCarPerSale[i], &typeOfCarPerSale[j]);

                // and in the discountGivenPerSale array
                swapBool(&discountGivenPerSale[i], &discountGivenPerSale[j]);

                // and lastly, do the same in the customerNames array

                // using a function to perform this swap would complicate this program a bit too much, but we
                //		can do the swap directly here quite easily
                char temp[201];
                // copy string from position i to the newly created temp variable
                strcpy(temp, customerNames[i]);
                // copy string from position j into position i
                strcpy(customerNames[i], customerNames[j]);
                // copy string from temp into position j
                strcpy(customerNames[j], temp);

            } // end of "if" statement

        } // end of second "for" loop

    } // end of first "for" loop

}

// specialised function that will display a row representing the sales data at a given array position
void printSalesDataAtPosition(int position) {

    // here, we're using the value found at position i in the typeOfTicketPerSale array
    //	as a position for the ticketPrices array to find out the price of the type of ticket
    //	the customer purchased this sale
    int typeOfCar = typeOfCarPerSale[position];
    // calculate the current sale's price
    float price = carAmountPerSale[position] * carPrices[typeOfCar];

    // calculate the display text for the discount; this is for the user's benefit
    char discountGivenText[4];
    // if a discount was given, then...
    if (discountGivenPerSale[position] == TRUE) {
        // set the display text to Yes
        strcpy(discountGivenText, "Yes");
        // and also modify the price
        price *= (1 - DISCOUNT_PERCENTAGE);
    }
    else {
        // set the display text to No
        strcpy(discountGivenText, "No");
    }

    // show the user the information related to each sale
    // I've broken down the "printf()" statement into several lines, so we
    //	can read it more easily, but it's still just one statement
    printf("Sale Index: %d | Sale Amount: %f | Type of Car: %s | "
           "Car Price: %f | Number of Cars: %hd | Car Color: %s |"
           "Discount Given: %s | Customer Name: %s\n",
            // please note that the above are three separate strings that will be glued together by the program
           position, price, carTypes[typeOfCar],
           carPrices[typeOfCar], carAmountPerSale[position],carColors,
           discountGivenText, customerNames[position]);

}

void menu_viewSales() {
    // sort the arrays before showing the sales data
    sortArraysByNumberOfCarsSoldPerSale();

    /* Variables - these are only used inside menu_viewSales() Function and nowhere else */
    // these two will contain the sum the total sales price and total number of tickets sold for all sales
    float totalSalesValue = 0;
    unsigned int CarsSold = 0;

    printf("All Sales Data:\n\n");

    // set up a for loop that will execute the block of code as many times as
    // indicated by the numberOfSales variable's value
    for (int i = 0; i < numberOfSales; i++) {

        // here, we're using the value found at position i in the typeOfTicketPerSale array
        //	as a position for the ticketPrices array to find out the price of the type of ticket
        //	the customer purchased this sale
        int typeOfCar = typeOfCarPerSale[i];
        // calculate the current sale's price
        float price = carAmountPerSale[i] * carPrices[typeOfCar];

        // use this function to print the row of sales data at position i
        printSalesDataAtPosition(i);

        // also add up the total sales value and total number of tickets sold
        totalSalesValue += price;
        CarsSold += carAmountPerSale[i];

    }

    // also display some overall statistics
    printf("\n%hd cars have been sold with a total value of %f GBP. There are %hd cars unsold.\n",
           carColors, CarsSold, totalSalesValue, carsAvailable);
}

void printSalesDataBasedOnCarSoldBetweenMinMax(unsigned short min, unsigned short max) {

    printf("Sales Data containing between %hd and %hd cars sold:\n\n", min, max);

    // searching through an array just means that you'll have a look that has an index
    //	variable starting with 0 and ending with the last position that contains a value
    //	in the array,and then you'll have an if statement that check every value
    //		if a value is found to match a certain condition, it means that we found
    //		what we're looking for, so the code inside the {} will be executed as a response
    //		to us finding that value in the array

    // go through every position that will contain a value in our ticketAmountPerSale array
    for (int i = 0; i < numberOfSales; i++) {

        // for every one of those positions, check if the value is between min and max
        if (min <= carAmountPerSale[i] && carAmountPerSale[i] <= max) {

            // print the sales data row at this position
            printSalesDataAtPosition(i);

        }

    }

}

void menu_viewCarStock() {

    printf("CAR BODY STYLE \n");
    unsigned short userChoice;

    int numberOfCars = sizeof(carPrices) / sizeof(float);
    for (int i = 0; i < numberOfCars; i++) {
        printf("%d - %s - %f \n",i, carTypes[i]);

        userChoice = getUnsignedShortFromConsole("What kind of car type would you like to see? Select from the options above:");
        switch (userChoice) {
            case 0:
                printf("SEDAN TYPE\t CAR PRICES \n\n");
                for (int i=0; i < TOTAL_NO_OF_CAR_MODELS i++;) {
                    printf("%hd = %s = %d GBP \n",i + 1, sedanTypeCarModelNames[i],sedanTypeCarPrices[i]);
                }
                break;

            case 1:
                printf("SUV TYPE\t CAR PRICES \n\n");
                for (int i=0; i < TOTAL_NO_OF_CAR_MODELS i++;) {
                    printf("%hd = %s = %d GBP \n",i + 1, suvTypeCarModelNames[i], suvTypeCarPrices[i]);
                }
                break;

            case 2:
                printf("HATCHBACK TYPE\t CAR PRICES \n\n");
                for (int i=0; i < TOTAL_NO_OF_CAR_MODELS i++;) {
                    printf("%hd = %s = %d GBP \n",i + 1, hatchbackTypeCarModelNames[i], hatchbackTypeCarPrices[i]);
                }
                break;

        }
    }
}


void menu_exit() {
    printf("Thank you for using this Car Sale program. Bye-bye!");

    // ensure no data is lost, save it to the file
    saveDataToFile();
}

/* Starting Point of our Program */
int main() {

    // see if there's any data in the file
    getDataFromFile();

    /* Variables - these are only used inside main() Function and nowhere else */
    // this will hold the user's choice when presented with the menu
    char userChoice;

    /* The rest of our Program */

    // loop/repeat the code between the immediately following curly brackets
    // for as long as the CONDITION found between the while()'s brackets
    // (see towards the end of void main() { ... }) evaluates to TRUE; in our case,
    // we'll keep repeating for as long as the user does not choose to Exit
    // i.e. pushes 'x' when asked to choose their next action
    do {
        clearScreen();

        menu_greetCustomer();
        menu_showMenu();

        // the return value supplied by the Function once it finishes will be stored in userChoice
        userChoice = getCharFromConsole("Please choose one: ");

        // for the user's benefit...
        clearScreen();

        // next, we check the user's choice and make a decision based on that
        switch (userChoice) {
            case MENU_OPTION_BUY_CARS:
                menu_buyCars();
                break;

            case MENU_OPTION_VIEW_SALES_DATA:
                menu_viewSales();
                break;

            case MENU_OPTION_VIEW_CAR_STOCK:
                menu_viewCarStock();
                break;

            case MENU_OPTION_EXIT:
                menu_exit();
                break;
        }

        // give the user a chance to read the previous output, allow them to continue when ready
        // this does not naturally exist in C
        pauseProgram(userChoice);

    } while (userChoice != MENU_OPTION_EXIT);

    // again, keep the screen clear
    clearScreen();

    // a good sentence almost never fails to warm the heart...
    printf("\n\nHave a good day!\n\n");

    return 0;
}
