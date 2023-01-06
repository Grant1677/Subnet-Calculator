//Author: Grant Long
#include <iostream>
#include <cmath>
using std::cout;
using std::cin;
using std::endl;

/* ---------Function Prototypes--------- */

/* Provides information/directions to user */
void intro();

/* Obtains IP from user */
void get_IP(char ipaddr[]);

/* Seperates the IP so it does not contain dots between the numbers */
void seperate_IP(char ipaddr[], char *ipNums[], char class_submask_addr[], char ipclass[]);

/* Converts the seperated IP address into binary representation */
void convert_IP_to_binary(char *ipNums[], int bin[4][8]);

/* Prints the binary representation of the IP address */
void print_IP(int bin[4][8]);

/* Obtains subnet mask prefix from user and deternmines the subnet mask address and which IP class the IP address is in */
void subnet_mask(int &submask, char subnetmask[], char ip_class[]);

/* Seperates the class A, B, or C subnet mask address so it does not contain dots between the numbers */
void seperate_class_subnet_mask(char class_submask_addr[], char *seperated_class_submask[], char ipclass[]);

/* Seperates the prefixed subnet mask address so it does not contain dots between the numbers */
void seperate_subnet_mask(char subnetmask[], char *seperated_submask[]);

/* Obtains all information from prev. functions and returns # of max subnets and hosts for a given IP address */
void subnet(int submask, int subnet_bin[4][8], char *seperated_submask[], int base_sub_bin[4][8], char *seperated_class_submask[], char ipclass[]);

int main(){
    
    /* Arrays to hold IP address and subnet masks addresses */
    char ipaddress[50];                     //holds IP address user gives
    char classABC_subnet_mask[50];          //holds subnet mask address based off of the IP given
    char subnetmask[50];                    //holds subnet mask address based off of the prefix given

    /* Arrays to hold the binary representation of the addresses */
    int binary[4][8] = {};                  //holds binary version of the IP address
    int classABC_subnet_binary[4][8] = {};  //holds either Class A, B, or C subnet mask address in binary based off of IP given
    int subnet_binary[4][8] = {};           //holds subnet mask address based off of the prefix given

    /* Arrays to hold the seperated versions of the IP address and subnet masks addresses */
    char *ip_numbers[4];                    //holds IP address without the '.' dots seperating them
    char *classABC_subnet_arr[4];           //holds subnet mask address without the '.' dots seperating them
    char *subnet_arr[4];                    //holds prefixed version subnet mask address without the '.' dots seperating them

    /* Misc. Variables */ 
    int submask_prefix = 0;                 //holds prefix for subnet mask
    char ip_class[1];                       //holds whether the IP is a Class A, B, or C address

    intro();
    cout << "\n+-------------------------------------------------------------+\n";
    cout << "|                         IP ADDRESS                          |\n"; 
    cout << "+-------------------------------------------------------------+\n";
    cout << "| Please enter a valid IP address. Your IP will be converted  |\n";
    cout << "| to binary upon entering in a(n) IP address into the field.  |\n";
    cout << "+-------------------------------------------------------------+\n";
    get_IP(ipaddress);
    seperate_IP(ipaddress, ip_numbers, classABC_subnet_mask, ip_class);
    convert_IP_to_binary(ip_numbers, binary);
    print_IP(binary);
    
    cout << "\n+-------------------------------------------------------------+\n";
    cout << "|                          SUBNETTING                         |\n"; 
    cout << "+-------------------------------------------------------------+\n";
    cout << "| Please enter a valid Subnet Prefix (Ex: /27). Upon entering |\n";
    cout << "| this into the field, you will receive a summary of          |\n";
    cout << "| information that combines your IP Class (A, B, or C) and    |\n";
    cout << "| Subnet Prefix to show you how many usable hosts and subnets |\n";
    cout << "| are available on your network.                              |\n";
    cout << "+-------------------------------------------------------------+\n";
    subnet_mask(submask_prefix, subnetmask, ip_class);
    seperate_class_subnet_mask(classABC_subnet_mask, classABC_subnet_arr, ip_class);
    seperate_subnet_mask(subnetmask, subnet_arr);
    subnet(submask_prefix, subnet_binary, subnet_arr, classABC_subnet_binary, classABC_subnet_arr, ip_class);

}

/* ---------Function Implementations--------- */
void intro(){
    cout << "\n+-------------------------------------------------------------+\n";
    cout << "|                      SUBNET CALCULATOR                      |\n";
    cout << "+-------------------------------------------------------------+\n";
    cout << "| This program converts an IPv4 address to binary and subnets |\n";
    cout << "| it based off of the subnet mask given. You will be given    |\n";
    cout << "| how many hosts and subnets there are in the network.        |\n";
    cout << "+-------------------------------------------------------------+\n";
}

void get_IP(char ipaddr[]){
    cout << "\nThe following are Decimal Address Ranges:\n";
    cout << "\nClass A: 1 - 126\n";
    cout << "Class B: 128 - 191\n";
    cout << "Class C: 192 - 223\n";
    cout << "\nIP Address: ";
    cin >> ipaddr;
}

void seperate_IP(char ipaddr[], char *ipNums[], char base_submask[], char ipclass[]){
    int i = 0;
    char *temp = strtok(ipaddr, "."); //seperate the IP where it finds a '.'
    while(temp){
        ipNums[i++] = temp;
        temp = strtok(NULL, ".");
    }

    //Class A
    if(atoi(ipNums[0]) >= 1 && atoi(ipNums[0]) <= 126){
        strcpy(base_submask, "255.0.0.0");
        strcpy(ipclass, "A");
    }

    //Class B
    else if(atoi(ipNums[0]) >= 128 && atoi(ipNums[0]) <= 191){
        strcpy(base_submask, "255.255.0.0");
        strcpy(ipclass, "B");
    }

    //Class C
    else if(atoi(ipNums[0]) >= 192 && atoi(ipNums[0]) <= 223){
        strcpy(base_submask, "255.255.255.0");
        strcpy(ipclass, "C");
    }  
}

void convert_IP_to_binary(char *ipNums[], int bin[4][8]){
    //clear memory of 2D array
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 8; j++){
            bin[i][j] = 0;
        }
    }

    for(int i = 0; i < 4; i++){
        int num = atoi(ipNums[i]);
        for(int j = 7; j >= 0; j--){
            bin[i][j] = num % 2; //Store remainder in 2D array
            num = num / 2;       //Divide the number by 2
        }
    }
}

void print_IP(int bin[4][8]){
    cout << "    Binary: ";
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 8; j++){
            cout << bin[i][j];
        }
        cout << " ";
    }
    cout << endl;
}

void subnet_mask(int &submask, char subnetmask[], char ip_class[]){
    
again:
    if(ip_class[0] == 'A'){
        cout << "\nThe following range of prefixes are available to use:\n";
        cout << "\nClass A: 8 - 32\n";
        
    }
    else if(ip_class[0] == 'B'){
        cout << "\nThe following range of prefixes are available to use:\n";
        cout << "Class B: 16 - 32\n";
    }
    else{
        cout << "\nThe following range of prefixes are available to use:\n";
        cout << "Class C: 24 - 32\n";
    }

    cout << "\nSubnet Prefix: /";
    cin >> submask;

    cout << "\n+-------------------------------------------------------------+\n\n";

    if(submask == 8){strcpy(subnetmask, "255.0.0.0");}
    else if(submask == 9){strcpy(subnetmask, "255.128.0.0");}
    else if(submask == 10){strcpy(subnetmask, "255.192.0.0");}
    else if(submask == 11){strcpy(subnetmask, "255.224.0.0");}
    else if(submask == 12){strcpy(subnetmask, "255.240.0.0");}
    else if(submask == 13){strcpy(subnetmask, "255.248.0.0");}
    else if(submask == 14){strcpy(subnetmask, "255.252.0.0");}
    else if(submask == 15){strcpy(subnetmask, "255.254.0.0");}
    else if(submask == 16){strcpy(subnetmask, "255.255.0.0");}
    else if(submask == 17){strcpy(subnetmask, "255.255.128.0");}
    else if(submask == 18){strcpy(subnetmask, "255.255.192.0");}
    else if(submask == 19){strcpy(subnetmask, "255.255.224.0");}
    else if(submask == 20){strcpy(subnetmask, "255.255.240.0");}
    else if(submask == 21){strcpy(subnetmask, "255.255.248.0");}
    else if(submask == 22){strcpy(subnetmask, "255.255.252.0");}
    else if(submask == 23){strcpy(subnetmask, "255.255.254.0");}
    else if(submask == 24){strcpy(subnetmask, "255.255.255.0");}
    else if(submask == 25){strcpy(subnetmask, "255.255.255.128");}
    else if(submask == 26){strcpy(subnetmask, "255.255.255.192");}
    else if(submask == 27){strcpy(subnetmask, "255.255.255.224");}
    else if(submask == 28){strcpy(subnetmask, "255.255.255.240");}
    else if(submask == 29){strcpy(subnetmask, "255.255.255.248");}
    else if(submask == 30){strcpy(subnetmask, "255.255.255.252");}
    else if(submask == 31){strcpy(subnetmask, "255.255.255.254");}
    else if(submask == 32){strcpy(subnetmask, "255.255.255.255");}
    else{
        cout << "Invalid subnet mask!";
        goto again;
    }

    cout << "Prefix /" << submask << " Subnet Mask:   " << subnetmask << endl;
}

void seperate_class_subnet_mask(char base_submask[], char *seperated_class_submask[], char ipclass[]){
    cout << "Class " << ipclass << " Subnet Mask:      " << base_submask << endl;
    int i = 0;
    char *temp = strtok(base_submask, "."); 
    while(temp){
        seperated_class_submask[i++] = temp;
        temp = strtok(NULL, ".");
    }
}

void seperate_subnet_mask(char subnetmask[], char *seperated_submask[]){
    int i = 0;
    char *temp = strtok(subnetmask, "."); 
    while(temp){
        seperated_submask[i++] = temp;
        temp = strtok(NULL, ".");
    }
}

void subnet(int submask, int subnet_bin[4][8], char *seperated_submask[], int base_sub_bin[4][8], char *seperated_class_submask[], char ipclass[]){
    //clear memory of 2D arrays
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 8; j++){
            base_sub_bin[i][j] = 0;
            subnet_bin[i][j] = 0;
        }
    }

    //Convert seperated class subnet mask address to binary
    for(int i = 0; i < 4; i++){
        int num = atoi(seperated_class_submask[i]);
        for(int j = 7; j >= 0; j--){
            base_sub_bin[i][j] = num % 2;
            num = num / 2;
        }
    }

    //Convert seperated subnet mask address to binary
    for(int i = 0; i < 4; i++){
        int num = atoi(seperated_submask[i]);
        for(int j = 7; j >= 0; j--){
            subnet_bin[i][j] = num % 2; 
            num = num / 2;       
        }
    }

    cout << "Prefix /" << submask << " Subnet Binary: ";
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 8; j++){
            cout << subnet_bin[i][j];
        }
        cout << " ";
    }
    cout << endl;

    cout << "Class " << ipclass << " Subnet Binary:    ";
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 8; j++){
            cout << base_sub_bin[i][j];
        }
        cout << " ";
    }
    cout << endl;

    //Calculations:
    int classABC_hosts, classABC_subnets;
    classABC_hosts = classABC_subnets = 0;

    int prefixed_hosts, prefixed_subnets, num_of_host_bits, num_of_network_bits, total_network_bits, total_host_bits;
    prefixed_hosts = prefixed_subnets = num_of_host_bits = num_of_network_bits = total_network_bits = total_host_bits = 0;

    //Set the number of network and host bits based off of the submask given
    if(submask >= 8 && submask < 16){
        num_of_network_bits = 8;
        num_of_host_bits = 24;
    }
    else if(submask >= 16 && submask < 24){
        num_of_network_bits = 16;
        num_of_host_bits = 16;
    }
    else if(submask >= 24 && submask <= 32){
        num_of_network_bits = 24;
        num_of_host_bits = 8;

    }
    
    //Class A, B, C base subnets and hosts
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 8; j++){
            if(base_sub_bin[i][j] == 1){
                classABC_subnets++;
            }
        }
    }
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 8; j++){
            if(base_sub_bin[i][j] == 0){
                classABC_hosts++;
            }
        }
    }
    
    //Prefix subnets and hosts
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 8; j++){
            if(subnet_bin[i][j] == 1){
                prefixed_subnets++;
            }
        }
    }
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 8; j++){
            if(subnet_bin[i][j] == 0){
                prefixed_hosts++;
            }
        }
    }

    cout << "\n+-------------------------------------------------------------+\n\n";
    cout << "Host bits from Class " << ipclass << ":   " << classABC_hosts << endl;
    cout << "Subnet bits from Class " << ipclass << ": " << classABC_subnets << endl;
    cout << "\n+-------------------------------------------------------------+\n\n";
    cout << "Host bits from /" << submask << ":   " << prefixed_hosts << endl;
    cout << "Subnet bits from /" << submask << ": " << prefixed_subnets << endl;
    cout << "\n+-------------------------------------------------------------+\n\n";
    total_network_bits = prefixed_subnets - classABC_subnets;
    total_host_bits = prefixed_hosts;

    int finalSubnets = pow(2, total_network_bits);
    int finalHosts = pow(2,total_host_bits) - 2;

    cout << "Total # of maximum Subnets:  (2^" << total_network_bits << ") = "  << finalSubnets << endl;
    cout << "Total # of hosts per Subnet: ((2^" << total_host_bits << ") - 2) = " << finalHosts << endl;
    cout << "\n+-------------------------------------------------------------+\n\n";
}
