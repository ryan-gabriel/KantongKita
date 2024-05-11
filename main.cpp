// Kelompok 5


#include<iostream>
#include<vector>
#include<map>
#include<string>
#include<chrono>
#include<iomanip>
#include<cstdio>
#include<conio.h>
#include<cstdlib>
#include<cctype>
#include<fstream>
#include<sstream>

using namespace std;

struct User {
    string username;
    string password;
};

struct pengeluaran{
    // data pengeluaran isinya => nama, harga, jumlah, keterangan
    vector<map<string,string>> dataPengeluaran;
    int tanggal, bulan, tahun;
    string waktu;
    double totalPengeluaran = 0;
    pengeluaran* next;
    pengeluaran* prev;
};

pengeluaran *pengeluaranHead, *pengeluaranTail;

struct pemasukan{
    int tanggal, bulan, tahun;
    string waktu;
    int harga;
    string nama;
    pemasukan *next;
    pemasukan *prev;
};

pemasukan *pemasukanHead, *pemasukanTail = NULL;


struct tabungan{
    int nominal, tanggal, bulan, tahun;
    string waktu;
    tabungan* next;
    tabungan* prev;
};

tabungan *tabunganHead, *tabunganTail = NULL;

struct penarikan{
    int tanggal,bulan,tahun;
    string waktu; 
    double nominalPenarikan;
    penarikan* next;
    penarikan* prev;
};

penarikan *penarikanHead, *penarikanTail=NULL;

string namaPemilik;

bool isLeapYear(int year) {
    return (year % 4 == 0);
}

bool isValidDate(int tanggal, int bulan, int tahun) {
    int hariDiBulan[] = {31, 28 + isLeapYear(tahun), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (tahun < 0 || bulan < 1 || bulan > 12 || tanggal < 1 || tanggal > hariDiBulan[bulan - 1]) {
        return false;
    }

    return true;
}

void addLoggedInUser(const string& filename, const User& user);
bool login(const vector<User>& users) {
    string inputUsername, inputPassword;
    std::cout << "Masukkan username: ";
    std::cin >> inputUsername;
    namaPemilik = inputUsername;
    std::cout << "Masukkan password: ";
    std::cin >> inputPassword;

    for (const User& user : users) {
        if (user.username == inputUsername && user.password == inputPassword) {
            std::cout << "Login berhasil!\n";
            addLoggedInUser("logged_in_users.csv", user);
            return true;
        }
    }
    std::cout << "Login gagal. Nama pengguna atau kata sandi tidak sesuai.\n";
    return false;
}

bool isPengeluaranEmpty(){
    return (pengeluaranHead == NULL);
};

bool isPemasukanEmpty(){
    return (pemasukanHead == NULL);
};

bool isTabunganEmpty(){
    return tabunganHead == NULL;
}

void addLoggedInUser(const string& filename, const User& user) {
    ofstream file(filename, ios::app);
    if (file.is_open()) {
        file << user.username << "," << user.password << "\n";
        file.close();
    } else {
        std::cout << "Gagal menyimpan ke file.\n";
    }
}

vector<User> readUsers(const string& filename) {
    vector<User> users;
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string username, password;
            getline(ss, username, ',');
            getline(ss, password, ',');
            users.push_back({username, password});
        }
        file.close();
    }
    return users;
}

void saveUsers(const string& filename, const vector<User>& users) {
    ofstream file(filename);
    if (file.is_open()) {
        for (const User& user : users) {
            file << user.username << "," << user.password << "\n";
        }
        file.close();
    } else {
        std::cout << "Gagal menyimpan ke file.\n";
    }
}

void signUp(vector<User>& users) {
    User newUser;
    std::cout << "Masukkan username baru: ";
    std::cin >> newUser.username;

    std::cout << "Masukkan password baru: ";
    std::cin >> newUser.password;

    if(newUser.password.length() <= 6){
        cout << "\033[1;31mPanjang password harus lebih dari 6 karakter! \033[0m" << endl;
        return;
    }

    for(const char& item : newUser.password){
        if(isdigit(item) || isalpha(item)){
            cout << "\033[1;31mPassword harus berupa simbol \033[0m" << endl;
            return;
        }
    }

    users.push_back(newUser);
    saveUsers("users.csv", users);
    std::cout << "Pendaftaran berhasil!\n";
}

void handleInvalidInput(){
    std::cin.clear();
    std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
    std::cout << "\033[1;31mInvalid Input!!! \033[0m" << endl << endl;
}

void printKeteranganPengeluaran(pengeluaran* newNode){
    for(int i = 0; i < newNode->dataPengeluaran.size(); i++){
            std::cout << "---------- Pengeluaran ke-" << i+1 << " ----------" << endl;
            if(newNode->dataPengeluaran[i]["nama"] != "-"){
                std::cout << "Nama Barang: " << newNode->dataPengeluaran[i]["nama"] << endl;
                std::cout << "jumlah: " << newNode->dataPengeluaran[i]["jumlah"] << endl;
                std::cout << "Harga Satuan: " << newNode->dataPengeluaran[i]["harga"] << endl;
                std::cout << "Total  Harga: " << stoi(newNode->dataPengeluaran[i]["harga"]) * stoi(newNode->dataPengeluaran[i]["jumlah"]) << endl;
            }else{
                std::cout << "Keterangan: " << newNode->dataPengeluaran[i]["keterangan"] << endl;
                std::cout << "Pengeluaran: " << newNode->dataPengeluaran[i]["harga"] << endl;
            }
        }
}

void addPengeluaran(){
    pengeluaran *newNode = new pengeluaran;
    newNode->next = NULL;
    char lanjut;
    map<string, string> entriBaru;
    do {
        int type;
        while (true) {
            std::cout << "1. Beli Barang" << endl;
            std::cout << "2. Biaya Jasa" << endl;
            std::cout << "Masukkan nomor sesuai jenis pengeluaran: "; std::cin >> type;
            std::cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (cin.fail()) {
                handleInvalidInput();
            }else {
                if (type == 1) {
                    std::cout << "Nama barang: "; 
                    getline(std::cin, entriBaru["nama"]);

                    while(true){
                        std::cout << "Jumlah barang yang dibeli: ";  
                        std::cin >> entriBaru["jumlah"];
                        if(atoi(entriBaru["jumlah"].c_str()) == 0){
                            std::cout << "\033[1;31mInvalid Input! Masukkan angka! \033[0m" << endl;
                        }else{
                            break;
                        }
                    }

                    while(true){
                        std::cout << "Harga satuan barang: ";  
                        std::cin >> entriBaru["harga"];
                        if(atoi(entriBaru["harga"].c_str()) == 0){
                            std::cout << "\033[1;31mInvalid Input! Masukkan angka! \033[0m" << endl;
                        }else{
                            break;
                        }
                    }
                    
                    entriBaru["keterangan"] = "-";   
                    newNode->totalPengeluaran += stoi(entriBaru["harga"]) * stoi(entriBaru["jumlah"]);
                    break;
                }
                else if (type == 2) {
                    std::cout << "Keterangan pengeluaran: "; 
                    getline(std::cin, entriBaru["keterangan"]);
                    while(true){
                        std::cout << "Banyak uang yang dikeluarkan: "; 
                        std::cin >> entriBaru["harga"];
                        if(atoi(entriBaru["harga"].c_str()) == 0){
                            std::cout << "\033[1;31mInvalid Input! Masukkan angka! \033[0m" << endl;
                        }else{
                            break;
                        }
                    }
                    entriBaru["nama"] = "-";
                    entriBaru["jumlah"] = "1";
                    newNode->totalPengeluaran += stoi(entriBaru["harga"]);
                    break;
                } else {
                    std::cout << "\033[1;31mInvalid Input! \033[0m" << endl << endl;
                }
            }
        }
        newNode->dataPengeluaran.push_back(entriBaru);
        std::cout << "Apakah anda ingin menambahkan pengeluaran lainnya? (y/n): "; 
        std::cin >> lanjut;
        std::cout << endl;
    } while (lanjut == 'Y' || lanjut == 'y');
   
    time_t currentTime = chrono::system_clock::to_time_t(chrono::system_clock::now());
    tm* localTime = localtime(&currentTime);
    newNode->tanggal = localTime->tm_mday;
    newNode->bulan = localTime->tm_mon+1;
    newNode->tahun = localTime->tm_year+1900;
    newNode->waktu = (localTime->tm_hour < 10 ? "0" : "") + to_string(localTime->tm_hour) + ":" + (localTime->tm_min < 10 ? "0" : "") + to_string(localTime->tm_min)+ ":" + (localTime->tm_sec < 10 ? "0" : "") + to_string(localTime->tm_sec);

    if(isPengeluaranEmpty()){
        newNode->prev = NULL;
        pengeluaranHead = pengeluaranTail = newNode;
    }
    else {
        newNode->prev = pengeluaranTail;
        pengeluaranTail -> next = newNode;
        pengeluaranTail = newNode;
    }
}

void addPemasukan(){
    string nama;
    int harga;
    std::cout << "Masukkan detail Pemasukkan\n";
    std::cout << "Nama : ";
    std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(std::cin, nama);
    while(true){
        std::cout << "Harga : ";
        std::cin >> harga;
        if(cin.fail()){
            handleInvalidInput();
        }
        else{
            break;
        }
    }
    
    pemasukan* newNode = new pemasukan;
    newNode->harga = harga;
    newNode->nama = nama;

    time_t currentTime = chrono::system_clock::to_time_t(chrono::system_clock::now());
    tm* localTime = localtime(&currentTime);
    newNode->tanggal = localTime->tm_mday;
    newNode->bulan = localTime->tm_mon+1;
    newNode->tahun = localTime->tm_year+1900;
    newNode->waktu = (localTime->tm_hour < 10 ? "0" : "") + to_string(localTime->tm_hour) + ":" + (localTime->tm_min < 10 ? "0" : "") + to_string(localTime->tm_min)+ ":" + (localTime->tm_sec < 10 ? "0" : "") + to_string(localTime->tm_sec);
    newNode->next = nullptr;

   if (pemasukanHead == nullptr) {
        pemasukanHead = newNode;
        pemasukanTail = newNode;
        newNode->prev = nullptr;
   }
   else {
        pemasukanTail->next = newNode;
        newNode->prev = pemasukanTail;
        pemasukanTail = newNode;
   }
}

void tampilanPemasukan() {
    if (pemasukanHead == NULL) {
        std::cout << "\033[1;31mTidak ada data pemasukan\033[0m" <<endl << endl;
        return;
    }
    std::cout << endl;
    pemasukan* temp = pemasukanHead;
    while (temp != NULL) {
        std::cout << "----------Pemasukan----------" << endl;
        std::cout << "Tanggal: " << temp->tanggal << " - " << temp->bulan << " - " << temp->tahun << endl;
        std::cout << "Waktu: " << temp->waktu << endl; 
        std::cout << "Nama: " << temp->nama << endl << "Pemasukan: " << temp->harga <<endl;
        temp = temp->next;
    }
    std::cout << endl;
}

void rataRataPengeluaran(){
    if(pengeluaranHead == nullptr) {
        std::cout << "\033[1;31mTidak ada data pengeluaran\033[0m" << endl;
        return;
    }

    int totalPengeluaran = 0;
    int jumlahPengeluaran = 0;

    pengeluaran* temp = pengeluaranHead;
    while (temp!= nullptr) {
        for (const auto& pengeluaran: temp->dataPengeluaran) {
            totalPengeluaran += stoi(pengeluaran.at("harga")) * stoi(pengeluaran.at("jumlah"));
            jumlahPengeluaran++;
        }
        temp = temp->next;
    }

    float rataRata = static_cast<double>(totalPengeluaran) / jumlahPengeluaran;
    
    std::cout << fixed << setprecision(2) <<"Rata-rata Pengeluaran adalah: " << rataRata << endl;
}

void rataRataPemasukan(){
    if (pemasukanHead == nullptr){
        std::cout << "\033[1;31mTidak Ada Pemasukan\033[0m" << endl;
        return;
    }
    float totalmasuk = 0;
    int count = 0;

    pemasukan* temp = pemasukanHead;
    while (temp != nullptr) {
        totalmasuk += temp->harga;
        count++;
        temp = temp->next;
    }
    float ratamasuk = totalmasuk / count;
    std::cout << fixed << setprecision(2) << "Rata-rata Pemasukan adalah: " << ratamasuk << endl;

    return;
};

void historiPengeluaranTerbaru(){
    if(isPengeluaranEmpty()){
        std::cout << "\033[1;31mTidak ada pengeluaran.\033[0m" << endl << endl;
        return;
    }
    pengeluaran* lastNode = pengeluaranTail;
    for(int i = 0; i < 2; i++){
        std::cout << endl;
        std::cout << "--------------------Data Pengeluaran--------------------" << endl;
        std::cout << "Tanggal: ";
        printf("%02d - %02d - %02d",lastNode->tanggal, lastNode->bulan, lastNode->tahun); 
        std::cout << endl;
        std::cout << "Waktu: " << lastNode->waktu << endl;
        printKeteranganPengeluaran(lastNode);
        std::cout << "----------------------------------------------" << endl;
        std::cout << fixed << setprecision(2) << "Total Pengeluaran: " << lastNode->totalPengeluaran << endl;
        std::cout << "----------------------------------------------";

        if(lastNode->prev == NULL){
            std::cout << endl << endl;
            return;
        }
        lastNode = lastNode->prev;
        std::cout << endl;
    }
    std::cout << endl;
    while (lastNode != NULL) {
        std::cout << "Klik tombol  \"Enter\" untuk melanjutkan atau tekan tombol \"esc\" untuk  kembali ke menu utama.";
        int userInput = _getch();
        if (userInput == 13) {
            std::cout << endl << endl;
            std::cout << "--------------------Data Pengeluaran--------------------" << endl;
            std::cout << "Tanggal: ";
            printf("%d - %d - %d",lastNode->tanggal, lastNode->bulan, lastNode->tahun);
            std::cout << endl;
            std::cout << "Waktu: " << lastNode->waktu << endl;
            printKeteranganPengeluaran(lastNode);
            std::cout << "----------------------------------------------" << endl;
            std::cout << fixed << setprecision(2) << "Total Pengeluaran: " << lastNode->totalPengeluaran << endl;
            std::cout << "----------------------------------------------";
            if (lastNode->prev == NULL) {
                std::cout << endl << endl;
                return;
            }
            std::cout << endl << endl;
            lastNode = lastNode->prev;
        } else if (userInput == 27) {
            std::cout << endl << endl;
            return;
        }
    }
}

string lowercase(string input){
    string lowerCaseInput;
    for(char c:input){
        lowerCaseInput += tolower(c);
    }
    return lowerCaseInput;
}

void searchPengeluaran(){
    if(isPengeluaranEmpty()){
        std::cout << "\033[1;31mTidak ada pengeluaran.\033[0m" << endl;
        return;
    }

    std::cout << "Jenis Pencarian" << endl;
    std::cout << "1. Berdasarkan \033[1mTanggal\033[0m" << endl;
    std::cout << "2. Berdasarkan \033[1mBulan\033[0m" << endl;
    std::cout << "3. Berdasarkan \033[1mTahun\033[0m" << endl;
    std::cout << "4. Berdasarkan \033[1mKeyword\033[0m" << endl;

    int pilihan;
    while(true){
        std::cout << "Masukkan jenis pencarian: "; std::cin >> pilihan;
        if(cin.fail() || pilihan < 1 || pilihan > 4 ){
            handleInvalidInput();
        }
        else{
            break;
        }
    }
    
    pengeluaran* newNode = pengeluaranHead;
    bool found = false;
    if(pilihan == 1){
        int tanggal, bulan, tahun;
        while(true){
            std::cout << "Masukkan tanggal, bulan, tahun pengeluaran (DD MM YY): "; std::cin >> tanggal >> bulan >> tahun;
            if(cin.fail() || tanggal > 31 || tanggal <= 0 || bulan > 12 || bulan <= 0 || tahun < 0){
                handleInvalidInput();
            }
            else if(!isValidDate(tanggal,bulan,tahun)){
                std::cout << "\033[1;31mTanggal tidak valid!!!\033[0m\n";
            }
            else{
                break;
            }
        }        
        while(newNode!=NULL){
            if(newNode->tanggal == tanggal && newNode->bulan == bulan && newNode->tahun == tahun){
                found = true;
                std::cout << "--------------------Data Pengeluaran--------------------" << endl;
                std::cout << "Tanggal: ";
                printf("%02d - %02d - %02d",newNode->tanggal, newNode->bulan, newNode->tahun); 
                std::cout << endl;
                std::cout << "Waktu: " << newNode->waktu << endl;
                printKeteranganPengeluaran(newNode);
                std::cout << "----------------------------------------------" << endl;
                std::cout << fixed << setprecision(2) << "Total Pengeluaran: " << newNode->totalPengeluaran << endl;
                std::cout << "----------------------------------------------" << endl << endl;
                
            }
            newNode = newNode->next;
        }

        if(!found){
            std::cout << "\033[1;31mTidak ada pengeluaran pada tanggal tersebut!\033[0m" << endl << endl;
            return;
        }
    }
    else if(pilihan == 2){
        int bulan, tahun;
        while(true){
            std::cout << "Masukkan bulan, tahun pengeluaran (MM YY): "; std::cin >> bulan >> tahun;
            if(cin.fail() || bulan > 12 || bulan <= 0 || tahun < 0){
                handleInvalidInput();
            }
            else{
                break;
            }
        }
        while(newNode!=NULL){
            if(newNode->bulan == bulan && newNode->tahun == tahun){
                found = true;
                std::cout << "--------------------Data Pengeluaran--------------------" << endl;
                std::cout << "Tanggal: ";
                printf("%02d - %02d - %02d",newNode->tanggal, newNode->bulan, newNode->tahun); 
                std::cout << endl;
                std::cout << "Waktu: " << newNode->waktu << endl;
                printKeteranganPengeluaran(newNode);
                std::cout << "----------------------------------------------" << endl;
                std::cout << fixed << setprecision(2) << "Total Pengeluaran: " << newNode->totalPengeluaran << endl;
                std::cout << "----------------------------------------------" << endl << endl;
            }
            newNode = newNode->next;
        }

        if(!found){
            std::cout << "\033[1;31mTidak ada pengeluaran pada bulan tersebut!\033[0m" << endl << endl;
            return;
        }
    }
    else if(pilihan == 3){
        int tahun;
        while(true){
            std::cout << "Masukkan tahun pengeluaran: "; std::cin >> tahun;
            if(cin.fail() || tahun < 0){
                handleInvalidInput();
            }
            else{
                break;
            }
        }
        while(newNode!=NULL){
            if(newNode->tahun == tahun){
                found = true;
                std::cout << "--------------------Data Pengeluaran--------------------" << endl;
                std::cout << "Tanggal: ";
                printf("%02d - %02d - %02d",newNode->tanggal, newNode->bulan, newNode->tahun); 
                std::cout << endl;
                std::cout << "Waktu: " << newNode->waktu << endl;
                printKeteranganPengeluaran(newNode);
                std::cout << "----------------------------------------------" << endl;
                std::cout << fixed << setprecision(2) << "Total Pengeluaran: " << newNode->totalPengeluaran << endl;
                std::cout << "----------------------------------------------" << endl << endl;
            }
            newNode = newNode->next;
        }

        if(!found){
            std::cout << "\033[1;31mTidak ada pengeluaran pada tahun tersebut!\033[0m" << endl << endl;
            return;
        }
    }

    else if (pilihan == 4) {
        string keyword;
        std::cout << "Masukkan keyword yang diinginkan: "; std::cin >> keyword;
        keyword = lowercase(keyword);

        while(newNode != NULL){
            for (int i = 0; i < newNode->dataPengeluaran.size(); i++) {
                if (lowercase(newNode->dataPengeluaran[i]["nama"]).find(keyword) != string::npos || lowercase(newNode->dataPengeluaran[i]["keterangan"]).find(keyword) != string::npos) {
                    if (!found) {
                        std::cout << "--------------------Data Pengeluaran--------------------" << endl;
                        std::cout << "Tanggal: ";
                        printf("%02d - %02d - %02d", newNode->tanggal, newNode->bulan, newNode->tahun); 
                        std::cout << endl;
                        std::cout << "Waktu: " << newNode->waktu << endl;
                        found = true;
                    }

                    std::cout << "---------- Pengeluaran ----------" << endl;
                    if (newNode->dataPengeluaran[i]["nama"] != "-") {
                        std::cout << "Nama Barang: " << newNode->dataPengeluaran[i]["nama"] << endl;
                        std::cout << "jumlah: " << newNode->dataPengeluaran[i]["jumlah"] << endl;
                        std::cout << "Harga Satuan: " << newNode->dataPengeluaran[i]["harga"] << endl;
                        std::cout << "Total  Harga: " << stoi(newNode->dataPengeluaran[i]["harga"]) * stoi(newNode->dataPengeluaran[i]["jumlah"]) << endl;
                    } else {
                        std::cout << "Keterangan: " << newNode->dataPengeluaran[i]["keterangan"] << endl;
                        std::cout << "Pengeluaran: " << newNode->dataPengeluaran[i]["harga"] << endl;
                    }
                }
            }

            newNode = newNode->next;
        }

        if (!found) {
            std::cout << "\033[1;31mTidak ada data pengeluaran yang cocok dengan kata kunci.\033[0m" << endl;
        }
    }
}

void searchPemasukan(){

    if(isPemasukanEmpty()){
        std::cout << "\033[1;31mTidak ada pemasukan.\033[0m" << endl;
        return;
    }

    std::cout << "Jenis Pencarian" << endl;
    std::cout << "1. Berdasarkan \033[1mTanggal\033[0m" << endl;
    std::cout << "2. Berdasarkan \033[1mBulan\033[0m" << endl;
    std::cout << "3. Berdasarkan \033[1mTahun\033[0m" << endl;
    std::cout << "4. Berdasarkan \033[1mKeyword\033[0m" << endl;

    int pilihan;
    while(true){
        std::cout << "Masukkan jenis pencarian: "; std::cin >> pilihan;
        if(cin.fail() || pilihan < 1 || pilihan > 4 ){
            handleInvalidInput();
        }
        else{
            break;
        }
    }
    
    pemasukan* newNode = pemasukanHead;
    bool found = false;
    if(pilihan == 1){
        int tanggal, bulan, tahun;
        while(true){
            std::cout << "Masukkan tanggal, bulan, tahun pemasukan (DD MM YY): "; std::cin >> tanggal >> bulan >> tahun;
            if(cin.fail() || tanggal > 31 || tanggal <= 0 || bulan > 12 || bulan <= 0 || tahun < 0){
                handleInvalidInput();
            }
            else if(!isValidDate(tanggal,bulan,tahun)){
                std::cout << "\033[1;31mTanggal tidak valid!!!\033[0m\n";
            }
            else{
                break;
            }
        }
        while(newNode!=NULL){
            if(newNode->tanggal == tanggal && newNode->bulan == bulan && newNode->tahun == tahun){
                found = true;
                std::cout << "--------------------Data Pemasukan--------------------" << endl;
                std::cout << "Tanggal: ";
                printf("%02d - %02d - %02d",newNode->tanggal, newNode->bulan, newNode->tahun); 
                std::cout << endl;
                std::cout << "Waktu: " << newNode->waktu << endl;
                std::cout <<"Keterangan Pemasukan: " << newNode->nama << endl;
                std::cout <<"Total Pemasukan: " << newNode->harga << endl;
                std::cout << "----------------------------------------------" << endl << endl;

            }
            newNode = newNode->next;
        }

        if(!found){
            std::cout << "\033[1;31mTidak ada pemasukan pada tanggal tersebut!\033[0m" << endl << endl;
            return;
        }
    }
    else if(pilihan == 2){
        int bulan, tahun;
        while(true){
            std::cout << "Masukkan bulan, tahun pemasukan (MM YY): "; std::cin >> bulan >> tahun;
            if(cin.fail() || bulan > 12 || bulan <= 0 || tahun < 0){
                handleInvalidInput();
            }
            else{
                break;
            }
        }
        while(newNode!=NULL){
            if(newNode->bulan == bulan && newNode->tahun == tahun){
                found = true;
                std::cout << "--------------------Data Pemasukan--------------------" << endl;
                std::cout << "Tanggal: ";
                printf("%02d - %02d - %02d",newNode->tanggal, newNode->bulan, newNode->tahun); 
                std::cout << endl;
                std::cout << "Waktu: " << newNode->waktu << endl;
                std::cout <<"Keterangan Pemasukan: " << newNode->nama << endl;
                std::cout <<"Total Pemasukan: " << newNode->harga << endl;
                std::cout << "----------------------------------------------" << endl << endl;
   
            }
            newNode = newNode->next;
        }

        if(!found){
            std::cout << "\033[1;31mTidak ada pemasukan pada bulan tersebut!\033[0m" << endl << endl;
            return;
        }
    }
    else if(pilihan == 3){
        int tahun;
        while(true){
            std::cout << "Masukkan tahun pemasukan: "; std::cin >> tahun;
            if(cin.fail() || tahun < 0){
                handleInvalidInput();
            }
            else{
                break;
            }
        }
        while(newNode!=NULL){
            if(newNode->tahun == tahun){
                found = true;
                std::cout << "--------------------Data Pengeluaran--------------------" << endl;
                std::cout << "Tanggal: ";
                printf("%02d - %02d - %02d",newNode->tanggal, newNode->bulan, newNode->tahun); 
                std::cout << endl;
                std::cout << "Waktu: " << newNode->waktu << endl;
                std::cout <<"Keterangan Pemasukan: " << newNode->nama << endl;
                std::cout <<"Total Pemasukan: " << newNode->harga << endl;
                std::cout << "----------------------------------------------" << endl << endl;
            }
            newNode = newNode->next;
        }

        if(!found){
            std::cout << "\033[1;31mTidak ada pemasukan pada tahun tersebut!\033[0m" << endl << endl;
            return;
        }
    }

    else if (pilihan == 4) {
        string keyword;
        std::cout << "Masukkan keyword yang diinginkan: "; std::cin >> keyword;
        keyword = lowercase(keyword);

        while(newNode != NULL){
            
            if (lowercase(newNode->nama).find(keyword) != string::npos) {
                if (!found) {
                    std::cout << "--------------------Data Pemasukan--------------------" << endl;
                    std::cout << "Tanggal: ";
                    printf("%02d - %02d - %02d", newNode->tanggal, newNode->bulan, newNode->tahun); 
                    std::cout << endl;
                    std::cout << "Waktu: " << newNode->waktu << endl;
                    found = true;
                }

                std::cout << "Keterangan Pemasukan: " << newNode->nama << endl;
                std::cout << "Total Pemasukan: " << newNode->harga << endl;
                std::cout << "----------------------------------------------" << endl << endl;
            }

            newNode = newNode->next;
        }

        if (!found) {
            std::cout << "\033[1;31mTidak ada data pemasukan yang cocok dengan kata kunci.\033[0m" << endl;
        }
    }
}

void hapusPengeluaran(){

    if(isPengeluaranEmpty()){
        std::cout << "\033[1;31mTidak ada pengeluaran\033[0m";
        return;
    }

    std::cout << "Jenis penghapusan:" << endl;
    std::cout << "1. Berdasarkan \033[1mTanggal\033[0m" << endl;
    std::cout << "2. Berdasarkan \033[1mBulan\033[0m" << endl;
    std::cout << "3. Berdasarkan \033[1mTahun\033[0m" << endl;
    std::cout << "4. Pengeluaran \033[1mTerbaru\033[0m" << endl;

    int pilihan;
    while(true){
        std::cout << "Masukkan jenis penghapusan: "; std::cin >> pilihan;
        if(cin.fail() || pilihan < 1 || pilihan > 4 ){
            handleInvalidInput();
        }
        else{
            break;
        }
    }

    pengeluaran* newNode = pengeluaranHead;
    pengeluaran* curNode;
    bool found = false;
    if(pilihan == 1){
        int tanggal, bulan, tahun;
        while(true){
            std::cout << "Masukkan tanggal, bulan, tahun pengeluaran (DD MM YY): "; std::cin >> tanggal >> bulan >> tahun;
            if(cin.fail() || tanggal > 31 || tanggal <= 0 || bulan > 12 || bulan <= 0 || tahun < 0){
                handleInvalidInput();
            }
            else if(!isValidDate(tanggal,bulan,tahun)){
                std::cout << "\033[1;31mTanggal tidak valid!!!\033[0m\n";
            }
            else{
                break;
            }
        }
        while(newNode!=NULL){
            if(newNode->tanggal == tanggal && newNode->bulan == bulan && newNode->tahun == tahun){
                found = true;
                curNode = newNode;
                if(pengeluaranHead == pengeluaranTail){
                    pengeluaranHead = pengeluaranTail = NULL;
                }
                else if(curNode == pengeluaranHead){
                    curNode->next->prev = NULL;
                    pengeluaranHead = pengeluaranHead->next;
                }
                else if(curNode == pengeluaranTail){
                    curNode->prev->next = NULL;
                    pengeluaranTail = curNode->prev;
                }
                else{
                    curNode->next->prev = curNode->prev;
                    curNode->prev->next = curNode->next;
                }
                delete curNode;
            }
            newNode = newNode->next;
        }

        if(!found){
            std::cout << "\033[1;31mTidak ada pengeluaran pada tanggal tersebut!\033[0m" << endl << endl;
            return;
        }
        std::cout << "\x1b[32mPengeluaran di bulan tersebut berhasil dihapus.\x1b[0m" << endl;
    }
    else if(pilihan == 2){
        int bulan, tahun;
        while(true){
            std::cout << "Masukkan bulan, tahun pengeluaran (MM YY): "; std::cin >> bulan >> tahun;
            if(cin.fail() || bulan > 12 || bulan <= 0 || tahun < 0){
                handleInvalidInput();
            }
            else{
                break;
            }
        }
        while(newNode!=NULL){
            if(newNode->bulan == bulan && newNode->tahun == tahun){
                found = true;
                curNode = newNode;
                
                if(pengeluaranHead == pengeluaranTail){
                    pengeluaranHead = pengeluaranTail = NULL;
                }
                else if(curNode == pengeluaranHead){
                    curNode->next->prev = NULL;
                    pengeluaranHead = pengeluaranHead->next;
                }
                else if(curNode == pengeluaranTail){
                    curNode->prev->next = NULL;
                    pengeluaranTail = curNode->prev;
                }
                else{
                    curNode->next->prev = curNode->prev;
                    curNode->prev->next = curNode->next;
                }
                delete curNode;
            }
            newNode = newNode->next;
        }

        if(!found){
            std::cout << "\033[1;31mTidak ada pengeluaran pada bulan tersebut!\033[0m" << endl << endl;
            return;
        }
        std::cout << "\x1b[32mPengeluaran di bulan tersebut berhasil dihapus.\x1b[0m" << endl;
    }
    else if(pilihan == 3){
        int tahun;
        while(true){
            std::cout << "Masukkan tahun pengeluaran: "; std::cin >> tahun;
            if(cin.fail() || tahun < 0){
                handleInvalidInput();
            }
            else{
                break;
            }
        }
        while(newNode!=NULL){
            if(newNode->tahun == tahun){
                found = true;
                curNode = newNode;
                
                if(pengeluaranHead == pengeluaranTail){
                    pengeluaranHead = pengeluaranTail = NULL;
                }
                else if(curNode == pengeluaranHead){
                    curNode->next->prev = NULL;
                    pengeluaranHead = pengeluaranHead->next;
                }
                else if(curNode == pengeluaranTail){
                    curNode->prev->next = NULL;
                    pengeluaranTail = curNode->prev;
                }
                else{
                    curNode->next->prev = curNode->prev;
                    curNode->prev->next = curNode->next;
                }
                delete curNode;

            }
            newNode = newNode->next;
        }

        if(!found){
            std::cout << "\033[1;31mTidak ada pengeluaran pada bulan tersebut!\033[0m" << endl << endl;
            return;
        }
        std::cout << "\x1b[32mPengeluaran di bulan tersebut berhasil dihapus.\x1b[0m" << endl;
    }
    else if(pilihan == 4){
        // kalau sisa satu pengeluaran masuk ke if pertama
        if(pengeluaranHead==pengeluaranTail) {
            pengeluaranHead = pengeluaranTail = NULL;
            delete newNode;
            std::cout << "\x1b[32mPengeluaran terbaru berhasil dihapus.\x1b[0m" << endl;
        }else{
            curNode->prev->next = NULL;
            pengeluaranTail = curNode->prev ;
            delete curNode;
            std::cout << "\x1b[32mPengeluaran terbaru berhasil dihapus.\x1b[0m" << endl;
        }
    }
}

void hapusPemasukan(){
    
    if(isPemasukanEmpty()){
        std::cout << "\033[1;31mTidak ada pemasukan\033[0m" << endl;
        return;
    }

    std::cout << "Jenis penghapusan:" << endl;
    std::cout << "1. Berdasarkan \033[1mTanggal\033[0m" << endl;
    std::cout << "2. Berdasarkan \033[1mBulan\033[0m" << endl;
    std::cout << "3. Berdasarkan \033[1mTahun\033[0m" << endl;
    std::cout << "4. Pengeluaran \033[1mTerbaru\033[0m" << endl;

    int pilihan;
    while(true){
        std::cout << "Masukkan jenis penghapusan: "; std::cin >> pilihan;
        if(cin.fail() || pilihan < 1 || pilihan > 4 ){
            handleInvalidInput();
        }
        else{
            break;
        }
    }

    pemasukan* newNode = pemasukanHead;
    pemasukan* curNode;
    bool found = false;
    if(pilihan == 1){
        int tanggal, bulan, tahun;
        while(true){
            std::cout << "Masukkan tanggal, bulan, tahun pemasukan (DD MM YY): "; std::cin >> tanggal >> bulan >> tahun;
            if(cin.fail() || tanggal > 31 || tanggal <= 0 || bulan > 12 || bulan <= 0 || tahun < 0){
                handleInvalidInput();
            }
            else if(!isValidDate(tanggal,bulan,tahun)){
                std::cout << "\033[1;31mTanggal tidak valid!!!\033[0m\n";
            }
            else{
                break;
            }
        }
        while(newNode!=NULL){
            if(newNode->tanggal == tanggal && newNode->bulan == bulan && newNode->tahun == tahun){
                found = true;
                curNode = newNode;
                
                if(pemasukanHead == pemasukanTail){
                    pemasukanHead = pemasukanTail = NULL;
                }
                else if(curNode == pemasukanHead){
                    curNode->next->prev = NULL; 
                    pemasukanHead = pemasukanHead->next;
                }
                else if(curNode == pemasukanTail){
                    curNode->prev->next = NULL;
                    pemasukanTail = curNode->prev;
                }
                else{
                    curNode->next->prev = curNode->prev;
                    curNode->prev->next = curNode->next;
                }
                delete curNode;
            }
            newNode = newNode->next;
        }

        if(!found){
            std::cout << "\033[1;31mTidak ada pemasukan pada tanggal tersebut!\033[0m" << endl << endl;
            return;
        }
        std::cout << "\x1b[32mPemasukan di bulan tersebut berhasil dihapus.\x1b[0m" << endl;
    }
    else if(pilihan == 2){
        int bulan, tahun;
        while(true){
            std::cout << "Masukkan bulan, tahun pemasukan (MM YY): "; std::cin >> bulan >> tahun;
            if(cin.fail() || bulan > 12 || bulan <= 0 || tahun < 0){
                handleInvalidInput();
            }
            else{
                break;
            }
        }
        while(newNode!=NULL){
            if(newNode->bulan == bulan && newNode->tahun == tahun){
                found = true;
                curNode = newNode;
                
                if(pemasukanHead == pemasukanTail){
                    pemasukanHead = pemasukanTail = NULL;
                }
                else if(curNode == pemasukanHead){
                    curNode->next->prev = NULL;
                    pemasukanHead = pemasukanHead->next;
                }
                else if(curNode == pemasukanTail){
                    curNode->prev->next = NULL;
                    pemasukanTail = curNode->prev;
                }
                else{
                    curNode->next->prev = curNode->prev;
                    curNode->prev->next = curNode->next;
                }
                delete curNode;

            }
            newNode = newNode->next;
        }

        if(!found){
            std::cout << "\033[1;31mTidak ada pemasukan pada bulan tersebut!\033[0m" << endl << endl;
            return;
        }
        std::cout << "\x1b[32mPemasukan di bulan tersebut berhasil dihapus.\x1b[0m" << endl;
    }
    else if(pilihan == 3){
        int tahun;
        while(true){
            std::cout << "Masukkan tahun pemasukan: "; std::cin >> tahun;
            if(cin.fail() || tahun < 0){
                handleInvalidInput();
            }
            else{
                break;
            }
        }
        while(newNode!=NULL){
            if(newNode->tahun == tahun){
                found = true;
                curNode = newNode;
                
                if(pemasukanHead == pemasukanTail){
                    pemasukanHead = pemasukanTail = NULL;
                }
                else if(curNode == pemasukanHead){
                    curNode->next->prev = NULL;
                    pemasukanHead = pemasukanHead->next;
                }
                else if(curNode == pemasukanTail){
                    curNode->prev->next = NULL;
                    pemasukanTail = curNode->prev;
                }
                else{
                    curNode->next->prev = curNode->prev;
                    curNode->prev->next = curNode->next;
                }
                delete curNode;

            }
            newNode = newNode->next;
        }

        if(!found){
            std::cout << "\033[1;31mTidak ada pemasukan pada bulan tersebut!\033[0m" << endl << endl;
            return;
        }
        std::cout << "\x1b[32mPemasukan di bulan tersebut berhasil dihapus.\x1b[0m" << endl;
    }
    else if(pilihan == 4){
        // kalau sisa satu pengeluaran masuk ke if pertama
        if(pemasukanHead==pemasukanTail) {
            pemasukanHead = pemasukanTail = NULL;
            delete newNode;
            std::cout << "\x1b[32mPemasukan terbaru berhasil dihapus.\x1b[0m" << endl;
        }else{
            curNode->prev->next = NULL;
            pemasukanTail = curNode->prev ;
            delete curNode;
            std::cout << "\x1b[32mPemasukan terbaru berhasil dihapus.\x1b[0m" << endl;
        }
    }
}



void addPengeluaranFirst(){
    pengeluaran *newNode = new pengeluaran();
    newNode->next = NULL;
    char lanjut;
    map<string, string> entriBaru;
    int tanggal, bulan, tahun, harga, jam, menit, detik;
    string waktu;
    while(true){
        while(true){
            std::cout << "Masukkan tanggal, bulan, tahun pengeluaran (DD MM YY): "; std::cin >> tanggal >> bulan >> tahun;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if(cin.fail() || tanggal > 31 || tanggal <= 0 || bulan > 12 || bulan <= 0 || tahun < 0){
                handleInvalidInput();
            }
            else if(!isValidDate(tanggal,bulan,tahun)){
                std::cout << "\033[1;31mTanggal tidak valid!!!\033[0m\n";
            }
            else{
                break;
            }
        }
        
        if((pengeluaranHead != NULL) && ((tanggal > pengeluaranHead->tanggal && bulan >= pengeluaranHead->bulan && tahun >= pengeluaranHead->tahun) ||(bulan > pengeluaranHead->bulan && tahun >= pengeluaranHead->tahun) || (tahun > pengeluaranHead->tahun))){
            std::cout << "Tanggal harus lebih awal dari pengeluaran pertama!!!" << endl;
        }else{
            break;
        }
    }

    newNode->tanggal = tanggal;
    newNode->bulan = bulan;
    newNode->tahun = tahun;

    while(true){
        try{
            std::cout << "Masukkan waktu pengeluaran (Jam Menit Detik): "; std::cin >> jam >> menit >> detik;
            if(cin.fail() || jam > 23 || jam <= 0 || menit > 59 || menit < 0 || detik > 59 || detik < 0){
                handleInvalidInput();
            }
            else{
                break;
            }
        }
        catch(...){
            handleInvalidInput();
        }
    }
    
    

    newNode->waktu = to_string(jam) + ":" + to_string(menit) + ":" + to_string(detik);

    do {
        int type;
        while (true) {
            std::cout << "1. Beli Barang" << endl;
            std::cout << "2. Biaya Jasa" << endl;
            std::cout << "Masukkan nomor sesuai jenis pengeluaran: "; std::cin >> type;
            std::cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (cin.fail()) {
                handleInvalidInput();
            }else {
                if (type == 1) {
                    std::cout << "Nama barang: "; 
                    getline(std::cin, entriBaru["nama"]);

                    while(true){
                        std::cout << "Jumlah barang yang dibeli: ";  
                        std::cin >> entriBaru["jumlah"];
                        if(atoi(entriBaru["jumlah"].c_str()) == 0){
                            std::cout << "\033[1;31mInvalid Input! Masukkan angka! \033[0m" << endl;
                        }else{
                            break;
                        }
                    }

                    while(true){
                        std::cout << "Harga satuan barang: ";  
                        std::cin >> entriBaru["harga"];
                        if(atoi(entriBaru["harga"].c_str()) == 0){
                            std::cout << "\033[1;31mInvalid Input! Masukkan angka! \033[0m" << endl;
                        }else{
                            break;
                        }
                    }
                    
                    entriBaru["keterangan"] = "-";   
                    newNode->totalPengeluaran += stoi(entriBaru["harga"]) * stoi(entriBaru["jumlah"]);
                    break;
                }
                else if (type == 2) {
                    std::cout << "Keterangan pengeluaran: "; 
                    getline(std::cin, entriBaru["keterangan"]);
                    while(true){
                        std::cout << "Banyak uang yang dikeluarkan: "; 
                        std::cin >> entriBaru["harga"];
                        if(atoi(entriBaru["harga"].c_str()) == 0){
                            std::cout << "\033[1;31mInvalid Input! Masukkan angka! \033[0m" << endl;
                        }else{
                            break;
                        }
                    }
                    entriBaru["nama"] = "-";
                    entriBaru["jumlah"] = "1";
                    newNode->totalPengeluaran += stoi(entriBaru["harga"]);
                    break;
                } else {
                    std::cout << "\033[1;31mInvalid Input! \033[0m" << endl << endl;
                }
            }
        }
        newNode->dataPengeluaran.push_back(entriBaru);
        std::cout << "Apakah anda ingin menambahkan pengeluaran lainnya? (y/n): "; 
        std::cin >> lanjut;
        std::cout << endl;
    } while (lanjut == 'Y' || lanjut == 'y');
   
    if(isPengeluaranEmpty()){
        newNode->prev = NULL;
        pengeluaranHead = pengeluaranTail = newNode;
    }
    else {
        pengeluaranHead->prev = newNode;
        newNode->next = pengeluaranHead;
        newNode->prev = NULL;
        pengeluaranHead = newNode;
    }
}

void addPemasukanFirst(){
    pemasukan *newNode = new pemasukan();
    newNode->next = NULL;
    int tanggal, bulan, tahun, harga, jam, menit, detik;
    string waktu, nama;

    while(true){
        while(true){
            std::cout << "Masukkan tanggal, bulan, tahun pemasukan (DD MM YY): "; std::cin >> tanggal >> bulan >> tahun;
            if(cin.fail() || tanggal > 31 || tanggal <= 0 || bulan > 12 || bulan <= 0 || tahun < 0){
                handleInvalidInput();
            }
            else if(!isValidDate(tanggal,bulan,tahun)){
                std::cout << "\033[1;31mTanggal tidak valid!!!\033[0m\n";
            }
            else{
                break;
            }
        }
        if((pemasukanHead != nullptr) && ((tanggal > pemasukanHead->tanggal && bulan >= pemasukanHead->bulan && tahun >= pemasukanHead->tahun) ||(bulan > pemasukanHead->bulan && tahun >= pemasukanHead->tahun) || (tahun > pemasukanHead->tahun))){
            std::cout << "\033[1;31mTanggal harus lebih awal dari pemasukan pertama!!!\033[0m" << endl;
        }else{
            break;
        }
    }

    newNode->tanggal = tanggal;
    newNode->bulan = bulan;
    newNode->tahun = tahun;

    while(true){
        std::cout << "Masukkan waktu pemasukan (Jam Menit Detik): "; std::cin >> jam >> menit >> detik;
        if(cin.fail() || jam > 23 || jam <= 0 || menit > 59 || menit < 0 || detik > 59 || detik < 0){
            handleInvalidInput();
        }
        else{
            break;
        }
    }

    newNode->waktu = to_string(jam) + ":" + to_string(menit) + ":" + to_string(detik);
    
    std::cout << "Masukkan detail Pemasukkan\n";
    std::cout << "Nama : ";
    std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(std::cin, nama);
    while(true){
        std::cout << "Harga : ";
        std::cin >> harga;
        if(cin.fail()){
            handleInvalidInput();
        }
        else{
            break;
        }
    }
    newNode->harga = harga;
    newNode->nama = nama;
    newNode->prev = nullptr;
    if (pemasukanHead == nullptr) {
        pemasukanHead = newNode;
        pemasukanTail = newNode;
   }
   else {
        newNode->next = pemasukanHead;
        pemasukanHead->prev = newNode;
        pemasukanHead = newNode;
   }
}

void addPengeluaranAfter(){
    pengeluaran *newNode = new pengeluaran();
    newNode->next = NULL;
    char lanjut;
    map<string, string> entriBaru;
    int tanggal, bulan, tahun, harga, jam, menit, detik;
    string waktu;
    while(true){
        while(true){
            std::cout << "Masukkan tanggal, bulan, tahun pengeluaran (DD MM YY): "; std::cin >> tanggal >> bulan >> tahun;
            if(cin.fail() || tanggal > 31 || tanggal <= 0 || bulan > 12 || bulan <= 0 || tahun < 0){
                handleInvalidInput();
            }
            else if(!isValidDate(tanggal,bulan,tahun)){
                std::cout << "\033[1;31mTanggal tidak valid!!!\033[0m\n";
            }
            else{
                break;
            }
        }
        if((pengeluaranHead != nullptr) && ((tanggal < pengeluaranHead->tanggal && bulan <= pengeluaranHead->bulan && tahun <= pengeluaranHead->tahun) ||(bulan < pengeluaranHead->bulan && tahun <= pengeluaranHead->tahun) || (tahun < pengeluaranHead->tahun))){
            std::cout << "Tanggal harus lebih dari pengeluaran pertama!!!" << endl;
        }else{
            break;
        }
    }
    newNode->tanggal = tanggal;
    newNode->bulan = bulan;
    newNode->tahun = tahun;

    while(true){
        std::cout << "Masukkan waktu pengeluaran (Jam Menit Detik): "; std::cin >> jam >> menit >> detik;
        if(cin.fail() || jam > 23 || jam <= 0 || menit > 59 || menit < 0 || detik > 59 || detik < 0){
            handleInvalidInput();
        }
        else{
            break;
        }
    }

    newNode->waktu = to_string(jam) + ":" + to_string(menit) + ":" + to_string(detik);

    do {
        int type;
        while (true) {
            std::cout << "1. Beli Barang" << endl;
            std::cout << "2. Biaya Jasa" << endl;
            std::cout << "Masukkan nomor sesuai jenis pengeluaran: "; std::cin >> type;
            std::cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (cin.fail()) {
                handleInvalidInput();
            }else {
                if (type == 1) {
                    std::cout << "Nama barang: "; 
                    getline(std::cin, entriBaru["nama"]);

                    while(true){
                        std::cout << "Jumlah barang yang dibeli: ";  
                        std::cin >> entriBaru["jumlah"];
                        if(atoi(entriBaru["jumlah"].c_str()) == 0){
                            std::cout << "\033[1;31mInvalid Input! Masukkan angka! \033[0m" << endl;
                        }else{
                            break;
                        }
                    }

                    while(true){
                        std::cout << "Harga satuan barang: ";  
                        std::cin >> entriBaru["harga"];
                        if(atoi(entriBaru["harga"].c_str()) == 0){
                            std::cout << "\033[1;31mInvalid Input! Masukkan angka! \033[0m" << endl;
                        }else{
                            break;
                        }
                    }
                    
                    entriBaru["keterangan"] = "-";   
                    newNode->totalPengeluaran += stoi(entriBaru["harga"]) * stoi(entriBaru["jumlah"]);
                    break;
                }
                else if (type == 2) {
                    std::cout << "Keterangan pengeluaran: "; 
                    getline(std::cin, entriBaru["keterangan"]);
                    while(true){
                        std::cout << "Banyak uang yang dikeluarkan: "; 
                        std::cin >> entriBaru["harga"];
                        if(atoi(entriBaru["harga"].c_str()) == 0){
                            std::cout << "\033[1;31mInvalid Input! Masukkan angka! \033[0m" << endl;
                        }else{
                            break;
                        }
                    }
                    entriBaru["nama"] = "-";
                    entriBaru["jumlah"] = "1";
                    newNode->totalPengeluaran += stoi(entriBaru["harga"]);
                    break;
                } else {
                    std::cout << "\033[1;31mInvalid Input! \033[0m" << endl << endl;
                }
            }
        }
        newNode->dataPengeluaran.push_back(entriBaru);
        std::cout << "Apakah anda ingin menambahkan pengeluaran lainnya? (y/n): "; 
        std::cin >> lanjut;
        std::cout << endl;
    } while (lanjut == 'Y' || lanjut == 'y');
   
    if(isPengeluaranEmpty()){
        newNode->prev = NULL;
        pengeluaranHead = pengeluaranTail = newNode;
    }
    else {
        pengeluaran *currentNode = pengeluaranHead;
        while(currentNode != NULL){
            if(currentNode->tanggal >= newNode->tanggal && currentNode->bulan >= newNode->bulan && currentNode->tahun >= newNode->tahun){
                newNode->prev = currentNode->prev;
                newNode->next = currentNode;
                currentNode->prev->next = newNode;
                currentNode->prev = newNode;
                break;
            }
            currentNode = currentNode->next;
        }
    }
    std::cout << "\x1b[32mPengeluaran berhasil ditambahkan.\x1b[0m" << endl;
}

void addPemasukanAfter(){
    pemasukan *newNode = new pemasukan;
    newNode->next = NULL;
    int tanggal, bulan, tahun, harga, jam, menit, detik;
    string waktu, nama;
    while(true){
        while(true){
            std::cout << "Masukkan tanggal, bulan, tahun pemasukan (DD MM YY): "; std::cin >> tanggal >> bulan >> tahun;
            if(cin.fail() || tanggal > 31 || tanggal <= 0 || bulan > 12 || bulan <= 0 || tahun < 0){
                handleInvalidInput();
            }
            else if(!isValidDate(tanggal,bulan,tahun)){
                std::cout << "\033[1;31mTanggal tidak valid!!!\033[0m\n";
            }
            else{
                break;
            }
        }
        if((pemasukanHead != nullptr) && ((tanggal < pemasukanHead->tanggal && bulan <= pemasukanHead->bulan && tahun <= pemasukanHead->tahun) ||(bulan < pemasukanHead->bulan && tahun <= pemasukanHead->tahun) || (tahun < pemasukanHead->tahun))){
            std::cout << "\033[1;31mTanggal tidak boleh lebih awal dari pemasukan pertama!!!\033[0m" << endl;
        }else{
            break;
        }
    }
    newNode->tanggal = tanggal;
    newNode->bulan = bulan;
    newNode->tahun = tahun;
    
    while(true){
        std::cout << "Masukkan waktu pemasukan (Jam Menit Detik): "; std::cin >> jam >> menit >> detik;
        if(cin.fail() || jam > 23 || jam <= 0 || menit > 59 || menit < 0 || detik > 59 || detik < 0){
            handleInvalidInput();
        }
        else{
            break;
        }
    }

    newNode->waktu = to_string(jam) + ":" + to_string(menit) + ":" + to_string(detik);
    
    std::cout << "Masukkan detail Pemasukkan\n";
    std::cout << "Nama : ";
    std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(std::cin, nama);
    while(true){
        std::cout << "Harga : ";
        if((!(std::cin >> harga))){
            handleInvalidInput();
        }
        else{
            break;
        }
    }
    newNode->harga = harga;
    newNode->prev = nullptr;
    if (pemasukanHead == nullptr) {
        pemasukanHead = newNode;
        pemasukanTail = newNode;
   }
    else {
        pemasukan *currentNode = pemasukanHead;
        while(currentNode != NULL){
            if(currentNode->tanggal >= newNode->tanggal && currentNode->bulan >= newNode->bulan && currentNode->tahun >= newNode->tahun){
                newNode->prev = currentNode->prev;
                newNode->next = currentNode;
                currentNode->prev->next = newNode;
                currentNode->prev = newNode;
                break;
            }
            currentNode = currentNode->next;
        }
    }
    std::cout << "\x1b[32mPemasukan berhasil ditambahkan.\x1b[0m" << endl << endl;
}

void hapusPengeluaranAwal(){
    if(isPengeluaranEmpty()){
        std::cout << "\033[1;31mTidak ada pengeluaran!!! \033[0m" << endl << endl;
        return;
    }

    pengeluaran* curNode = pengeluaranHead;
    if(pengeluaranHead == pengeluaranTail){
        pengeluaranHead = pengeluaranTail = NULL;
    }
    else{
        pengeluaranHead->next->prev = NULL;
        pengeluaranHead = pengeluaranHead->next;
    }
    delete curNode;
    std::cout << "\x1b[32mPengeluaran awal berhasil dihapus.\x1b[0m" << endl << endl;
}

void hapusPemasukanAwal(){
    if(isPemasukanEmpty()){
        std::cout << "\033[1;31mTidak ada pemasukan\033[0m" << endl;
        return;
    }
    pemasukan* curNode = pemasukanHead;
    if(pemasukanHead == pemasukanTail){
        pemasukanHead = pemasukanTail = NULL;
    }
    else{
        pemasukanHead->next->prev = NULL;
        pemasukanHead = pemasukanHead->next;
    }
    delete curNode;
    std::cout << "\x1b[32mPemasukan awal berhasil dihapus.\x1b[0m" << endl << endl;
}

void hapusPengeluaranAkhir(){
    if(isPengeluaranEmpty()){
        std::cout << "\033[1;31mTidak ada pengeluaran!!! \033[0m" << endl << endl;
        return;
    }

    pengeluaran* curNode = pengeluaranTail;
    if(pengeluaranHead == pengeluaranTail){
        pengeluaranHead = pengeluaranTail = NULL;
    }
    else{
        pengeluaranTail->prev->next = NULL;
        pengeluaranTail = pengeluaranTail->prev;
    }
    delete curNode;
    std::cout << "\x1b[32mPengeluaran terakhir berhasil dihapus.\x1b[0m" << endl << endl;
}

void hapusPemasukanAkhir(){
    if(isPemasukanEmpty()){
        std::cout << "\033[1;31mTidak ada pemasukan!!! \033[0m" << endl << endl;
        return;
    }

    pemasukan* curNode = pemasukanTail;
    if(pemasukanHead==pemasukanTail){
        pemasukanHead = pemasukanTail = NULL;
    }
    else{
        pemasukanTail->prev->next = NULL;
        pemasukanTail = pemasukanTail->prev;
    }
    delete curNode;
    std::cout << "\x1b[32mPemasukan terakhir berhasil dihapus.\x1b[0m" << endl << endl;
}

double angkaSaldoTabungan(){
    string filename = "SaldoTabungan.csv";
    ifstream file(filename);
    if(file.good()){
        vector<string> lines;
        string line;
        while (getline(file, line)) {
            lines.push_back(line);
        }
        file.close();
        bool foundName = false;
        int indexLine = 0;
        for(const string& item: lines) {
            size_t koma = item.find(",");
            if(koma!=string::npos){
                if(item.substr(0,koma) == namaPemilik){
                    return stod(item.substr(koma + 1)); 
                }
            }
            indexLine++;
        }
        return 0;
    }
    else{
        return 0;
    }
    
}

void saveSaldoTabungan(int tabungan, bool tambah) {
    string filename = "SaldoTabungan.csv";
    ifstream file(filename);

    vector<string> lines;
    string line;

    while (getline(file, line)) {
        lines.push_back(line);
    }
    file.close();
    bool foundName = false;
    int indexLine = 0;
    for(const string& item: lines) {
        size_t koma = item.find(",");
        if(koma!=string::npos){
            if(item.substr(0,koma) == namaPemilik){
                lines[indexLine] = tambah? namaPemilik + "," +to_string(stoi(item.substr(koma+1)) + tabungan) : namaPemilik + "," +to_string(stoi(item.substr(koma+1)) - tabungan) ;
                foundName = true;
                break; 
            }
        }
        indexLine++;
    }

    if(!foundName){
        string newData = namaPemilik + "," + to_string(tabungan);
        lines.push_back(newData);
    }

    ofstream outFile(filename);
    for (const string& line : lines) {
        outFile << line << endl;
    }
    outFile.close();
}


void addTabungan(){
    int nominal;
    while(true){
        std::cout << "Masukkan nominal tabungan: "; std::cin >> nominal;
        if(cin.fail() || nominal <= 0){
            handleInvalidInput();
        }
        else{
            break;
        }
    }
    saveSaldoTabungan(nominal, true);
    tabungan* newNode = new tabungan;
    newNode->nominal = nominal;
    newNode->next = NULL;

    time_t currentTime = chrono::system_clock::to_time_t(chrono::system_clock::now());
    tm* localTime = localtime(&currentTime);
    newNode->tanggal = localTime->tm_mday;
    newNode->bulan = localTime->tm_mon+1;
    newNode->tahun = localTime->tm_year+1900;
    newNode->waktu = (localTime->tm_hour < 10 ? "0" : "") + to_string(localTime->tm_hour) + ":" + (localTime->tm_min < 10 ? "0" : "") + to_string(localTime->tm_min)+ ":" + (localTime->tm_sec < 10 ? "0" : "") + to_string(localTime->tm_sec);
    if(tabunganHead != NULL){

        newNode->prev = tabunganTail;
        tabunganTail->next = newNode;
        tabunganTail = newNode;
    }
    else if(tabunganHead == tabunganTail){
        tabunganHead = tabunganTail = newNode;
    }
    else{
        tabunganTail->next = newNode;
        newNode->prev = tabunganTail;
        tabunganTail = newNode;
    }
    std::cout << "\x1b[32mTabungan Berhasil ditambahkan\x1b[0m" << endl << endl;
}

void saveNodeTabungan(){
    string namaFile = "nodeTabungan_"+ namaPemilik + ".csv";
    ofstream file(namaFile);
    tabungan* curr = tabunganHead;
    while (curr != NULL) {
        file << curr->tanggal << "," << curr->bulan << "," << curr->tahun << "," << curr->waktu << "," << curr->nominal << endl;
        curr = curr->next;
    }
}

void loadNodeTabungan() {
    string namaFile = "nodeTabungan_" + namaPemilik + ".csv";
    ifstream file(namaFile);
    string line;
    vector<string> lines;
    
    if (file.is_open()) {
        while (getline(file, line)) {
            if (!line.empty()) {
                lines.push_back(line);
            }
        }
        file.close();

        for (const string& item : lines) {
            tabungan* newNode = new tabungan;
            stringstream ss(item);
            string temp;

            getline(ss, temp, ',');
            newNode->tanggal = stoi(temp);
            getline(ss, temp, ',');
            newNode->bulan = stoi(temp);
            getline(ss, temp, ',');
            newNode->tahun = stoi(temp);
            getline(ss, temp, ',');
            newNode->waktu = temp;
            getline(ss, temp, ',');
            newNode->nominal = stoi(temp);

            if (tabunganHead == NULL) {
                tabunganHead = tabunganTail = newNode;
            } else {
                newNode->prev = tabunganTail;
                tabunganTail->next = newNode;
                tabunganTail = newNode;
            }
        }
        if (tabunganTail != NULL) {
            tabunganTail->next = NULL;
        }
    } else {
        return;
    }
}


void historiTabungan(){
    if(tabunganHead == NULL) {
        std::cout << "Tidak ada riwayat tabungan." << endl;
        return;
    }
    
    tabungan* newNode = tabunganHead;
    while(newNode != NULL){
        std::cout << "------------------------------------------------" << endl;
        std::cout << "Tanggal: " << newNode->tanggal << " - " << newNode->bulan << " - " << newNode->tahun << endl;
        std::cout << "Waktu: " << newNode->waktu << endl;
        std::cout << "Nominal: Rp." << newNode->nominal << endl;
        newNode = newNode->next;
    }
    std::cout << endl;
}


void loadPemasukan() {
    string namaFile = "nodePemasukan_" + namaPemilik + ".csv";
    ifstream file(namaFile);
    string line;
    vector<string> lines;
    
    if (file.is_open()) { 
        while (getline(file, line)) {
            if (!line.empty()) {
                lines.push_back(line);
            }
        }
        file.close();

        for (const string& item : lines) {
            pemasukan* newNode = new pemasukan;
            stringstream ss(item);
            string temp;

            getline(ss, temp, ',');
            newNode->tanggal = stoi(temp);
            getline(ss, temp, ',');
            newNode->bulan = stoi(temp);
            getline(ss, temp, ',');
            newNode->tahun = stoi(temp);
            getline(ss, newNode->waktu, ',');
            getline(ss, temp, ',');
            newNode->harga = stoi(temp);
            getline(ss, newNode->nama);

            if (pemasukanHead == NULL) {
                pemasukanHead = pemasukanTail = newNode;
            } else {
                newNode->prev = pemasukanTail;
                pemasukanTail->next = newNode;
                pemasukanTail = newNode;
            }
        }
        if (pemasukanTail != NULL) {
            pemasukanTail->next = NULL;
        }
    } else {
        return;
    }
}   

void saveNodePemasukan() {
    string namaFile = "nodePemasukan_" + namaPemilik + ".csv";
    ofstream file(namaFile);
    pemasukan* curr = pemasukanHead;

    while (curr != NULL) {
        file << curr->tanggal << "," << curr->bulan << "," << curr->tahun << "," << curr->waktu << "," << curr->harga << "," << curr->nama << endl;
        curr = curr->next;
    }
}

void saveNodePengeluaran() {
    string namaFile = "nodePengeluaran_" + namaPemilik + ".csv";
    ofstream file(namaFile);

    pengeluaran* curr = pengeluaranHead;

    while (curr != nullptr) {
        file << curr->tanggal << "," << curr->bulan << "," << curr->tahun << "," << curr->waktu << "," << curr->totalPengeluaran;

        for (const auto& item : curr->dataPengeluaran) {
            file << "," << item.at("harga") << "," << item.at("jumlah") << "," << item.at("keterangan") << "," << item.at("nama");
        }

        file << endl;
        curr = curr->next;
    }

    file.close();
}

void loadPengeluaran() {
    string namaFile = "nodePengeluaran_" + namaPemilik + ".csv";
    ifstream file(namaFile);

    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            pengeluaran* newNode = new pengeluaran;
            newNode->next = NULL;
            stringstream ss(line);
            string temp;

            getline(ss, temp, ',');
            newNode->tanggal = stoi(temp);
            getline(ss, temp, ',');
            newNode->bulan = stoi(temp);
            getline(ss, temp, ',');
            newNode->tahun = stoi(temp);
            getline(ss, newNode->waktu, ',');
            getline(ss, temp, ',');
            newNode->totalPengeluaran = stod(temp);

            while (getline(ss, temp, ',')) {
                map<string, string> dataItem;
                dataItem["harga"] = temp;
                getline(ss, temp, ',');
                dataItem["jumlah"] = temp;
                getline(ss, temp, ',');
                dataItem["keterangan"] = temp;
                getline(ss, temp, ',');
                dataItem["nama"] = temp;
                newNode->dataPengeluaran.push_back(dataItem);
            }

            if (pengeluaranHead == NULL) {
                newNode->prev = NULL;
                pengeluaranHead = pengeluaranTail = newNode;
            } else {
                newNode->prev = pengeluaranTail;
                pengeluaranTail->next = newNode;
                pengeluaranTail = newNode;
            }
        }

        file.close();
    } else {
        return;
    }
}


void tarikTabungan(){
    double nominal;
    while(true){
        std::cout << "Nominal yang ingin ditarik: "; std::cin >> nominal;
        if(cin.fail()){
            handleInvalidInput();
        }
        else if(nominal <= 0){
            std::cout << "\033[1;31mPenarikan Tidak Bisa sama dengan atau kurang dari 0 \033[0m" << endl;
        }
        else{
            break;
        }
    }
    if(angkaSaldoTabungan() - nominal < 0){
        std::cout << "\033[1;31mSaldo Tabungan tidak mencukupi!\033[0m" << endl;
        return;
    }

    penarikan* newNode = new penarikan;
    newNode->nominalPenarikan = nominal;
    saveSaldoTabungan(nominal,false);
    time_t currentTime = chrono::system_clock::to_time_t(chrono::system_clock::now());
    tm* localTime = localtime(&currentTime);
    newNode->tanggal = localTime->tm_mday;
    newNode->bulan = localTime->tm_mon+1;
    newNode->tahun = localTime->tm_year+1900;
    newNode->waktu = (localTime->tm_hour < 10 ? "0" : "") + to_string(localTime->tm_hour) + ":" + (localTime->tm_min < 10 ? "0" : "") + to_string(localTime->tm_min)+ ":" + (localTime->tm_sec < 10 ? "0" : "") + to_string(localTime->tm_sec);
    
    newNode->next = NULL;

    if(penarikanHead == NULL){
        newNode->prev = NULL;
        penarikanHead= penarikanTail = newNode;
    }
    else{
        penarikanTail->next = newNode;
        newNode->prev = penarikanTail;
        penarikanTail = newNode;
    }
    std::cout << "\x1b[32mPenarikan berhasil dilakukan\x1b[0m" << endl << endl;
}

void saveNodePenarikan(){
    string filename = "nodePenarikan_" + namaPemilik + ".csv";
    ofstream file (filename);
    penarikan* curr = penarikanHead;

    while (curr != NULL) {
        file << curr->tanggal << "," << curr->bulan << "," << curr->tahun << "," << curr->waktu << "," << curr->nominalPenarikan << endl;
        curr = curr->next;
    }
}

void loadPenarikan() {
    string namaFile = "nodePenarikan_" + namaPemilik + ".csv";
    ifstream file(namaFile);
    string line;
    vector<string> lines;
    
    if (file.is_open()) {
        while (getline(file, line)) {
            if (!line.empty()) {
                lines.push_back(line);
            }
        }
        file.close();

        for (const string& item : lines) {
            penarikan* newNode = new penarikan;
            stringstream ss(item);
            string temp;

            getline(ss, temp, ',');
            newNode->tanggal = stoi(temp);
            getline(ss, temp, ',');
            newNode->bulan = stoi(temp);
            getline(ss, temp, ',');
            newNode->tahun = stoi(temp);
            getline(ss, newNode->waktu, ',');
            getline(ss, temp, ',');
            newNode->nominalPenarikan = stod(temp);

            if (penarikanHead == NULL) {
                penarikanHead = penarikanTail = newNode;
            } else {
                newNode->prev = penarikanTail;
                penarikanTail->next = newNode;
                penarikanTail = newNode;
            }
        }
        if (penarikanTail != NULL) {
            penarikanTail->next = NULL;
        }
    } else {
        return;
    }
} 

void tampilanPenarikan(){
    if(penarikanHead == NULL) {
        std::cout << "\033[1;31mTidak ada riwayat penarikan.\033[0m" << endl;
        return;
    }
    penarikan* newNode = penarikanHead;
    while(newNode != NULL){
        std::cout << "------------------------------------------------" << endl;
        std::cout << "Tanggal: " << newNode->tanggal << " - " << newNode->bulan << " - " << newNode->tahun << endl;
        std::cout << "Waktu: " << newNode->waktu << endl;
        std::cout << "Nominal: Rp." << newNode->nominalPenarikan << endl;
        newNode = newNode->next;
    }
    std::cout << endl;
}

void tampilkanSaldo(){
    string filename = "SaldoTabungan.csv";
    ifstream file(filename);
    vector<string> lines;
    string line;

    while (getline(file, line)) {
        lines.push_back(line);
    }
    file.close();

    for(const string& item: lines) {
        size_t koma = item.find(",");
        if(koma!=string::npos){
            if(item.substr(0,koma) == namaPemilik){
                std::cout << "Saldo tabungan anda adalah Rp.\x1b[32m" << stoi(item.substr(koma+1)) << "\x1b[0m" << endl;
                return;
            }
        }
    }

    std::cout << "\033[1;31mAnda belum memiliki tabungan.\033[0m" << endl;
}

int main(){
    vector<User> users = readUsers("users.csv");

    while(true){
        string choice;
        std::cout << "Apakah Anda sudah memiliki akun? (y/n): ";
        std::cin >> choice;

        if (choice == "n" || choice == "N") {
            signUp(users);
        }
        else if (choice == "y" || choice == "Y") {
            if (login(users)) {
                std::cout << "Akses diterima.\n";
                break;
            }
            else {
                std::cout << "Akses ditolak.\n";
            }
        } 
        else {
            handleInvalidInput();
        }
    }

    loadNodeTabungan();
    loadPemasukan();
    loadPengeluaran();
    loadPenarikan();
    while(true){
        std::cout << "Selamat Datang di \033[1mKantongKita\033[0m !!!" << endl;
        std::cout << "1. Menu Pengeluaran" << endl;
        std::cout << "2. Menu Pemasukan" << endl;
        std::cout << "3. Menu Tabungan" << endl;
        std::cout << "4. Rata-rata pemasukan dan pengeluaran" << endl;
        std::cout << "5. Keluar" << endl;
        
        int choice;
        std::cout << "Masukkan pilihan Anda : "; std::cin >> choice;
        std::cout << endl;
        if(cin.fail()){
            handleInvalidInput();
        }
        else{
            if(choice == 1){
                while(true){
                    std::cout << "Menu \033[1mPengeluaran\033[0m" << endl;
                    std::cout << "1. Catat Pengeluaran" << endl;
                    std::cout << "2. Catat Pengeluaran Pertama" << endl;
                    std::cout << "3. Catat Pengeluaran Antara Pengeluaran Yang Sudah Ada" << endl;
                    std::cout << "4. Histori Pengeluaran Terbaru" << endl;
                    std::cout << "5. Hapus Data Pengeluaran" << endl;
                    std::cout << "6. Hapus Data Pengeluaran Awal" << endl;
                    std::cout << "7. Hapus Data Pengeluaran Terakhir" << endl;
                    std::cout << "8. Pencarian Data Pengeluaran" << endl;
                    std::cout << "9. Kembali" << endl;
                    std::cout << "Masukkan pilihan Anda : "; std::cin >> choice;
                    if(cin.fail() || choice < 1 || choice > 9){
                        handleInvalidInput();
                    }
                    else if(choice == 1){
                        addPengeluaran();
                        std::cout << "\033[32mPengeluaran Sudah Disimpan.\033[0m" << endl << endl;
                        saveNodePengeluaran();
                        break;
                    }
                    else if(choice == 2){
                        addPengeluaranFirst();
                        saveNodePengeluaran();
                        break;
                    }
                    else if(choice == 3){
                        addPengeluaranAfter();
                        saveNodePengeluaran();
                        break;
                    }
                    else if(choice == 4){
                        historiPengeluaranTerbaru();
                        break;
                    }
                    else if(choice == 5){
                        hapusPengeluaran();
                        saveNodePengeluaran();
                        break;
                    }
                    else if(choice == 6){
                        hapusPengeluaranAwal();
                        saveNodePengeluaran();
                        break;
                    }
                    else if(choice == 7){
                        hapusPengeluaranAkhir();
                        saveNodePengeluaran();
                        break;
                    }
                    else if(choice == 8){
                        searchPengeluaran();
                        break;
                    }
                    else if(choice == 9){
                        std::cout << endl;
                        break;
                    }
                }
            }
            else if(choice == 2){
                while(true){
                    std::cout << "Menu \033[1mPemasukan\033[0m" << endl;
                    std::cout << "1. Catat Pemasukan" << endl;
                    std::cout << "2. Catat Pemasukan Pertama" << endl;
                    std::cout << "3. Catat Pemasukan Antara Pemasukan Yang Sudah Ada" << endl;
                    std::cout << "4. Ringkasan Seluruh Pemasukan" << endl;
                    std::cout << "5. Pencarian Data Pemasukan" << endl;
                    std::cout << "6. Hapus Data Pemasukan" << endl;
                    std::cout << "7. Hapus Data Pemasukan Awal" << endl;
                    std::cout << "8. Hapus Data Pemasukan Terakhir" << endl;
                    std::cout << "9. Kembali" << endl;
                    std::cout << "Masukkan pilihan Anda : "; std::cin >> choice;
                    if(cin.fail() || choice < 1 || choice > 9){
                        handleInvalidInput();
                    }
                    else if(choice == 1){
                        addPemasukan();
                        saveNodePemasukan();
                        break;
                    }
                    else if(choice == 2){
                        addPemasukanFirst();
                        saveNodePemasukan();
                        break;
                    }
                    else if(choice == 3){
                        addPemasukanAfter();
                        saveNodePemasukan();
                        break;
                    }
                    else if(choice == 4){
                        tampilanPemasukan();
                        break;
                    }
                    else if(choice == 5){
                        searchPemasukan();
                    }
                    else if(choice == 6){
                        hapusPemasukan();
                        saveNodePemasukan();
                        break;
                    }
                    else if(choice == 7){
                        hapusPemasukanAwal();
                        saveNodePemasukan();
                        break;
                    }
                    else if(choice == 8){
                        hapusPemasukanAkhir();
                        saveNodePemasukan();
                        break;
                    }
                    else if(choice == 9){
                        std::cout << endl;
                        break;
                    }
                }
            }
            else if(choice == 3){
                while(true){
                    std::cout << "Menu \033[1mTabungan\033[0m" << endl;
                    std::cout << "1. Tambah Tabungan" << endl;
                    std::cout << "2. Riwayat Tabungan" << endl;
                    std::cout << "3. Tarik Tabungan" << endl;
                    std::cout << "4. Riwayat Penarikan Tabungan " << endl;
                    std::cout << "5. Lihat Saldo Tabungan" << endl;
                    std::cout << "6. Kembali" << endl;
                    std::cout << "Masukkan pilihan Anda : "; std::cin >> choice;
                    if(cin.fail() || choice < 1 || choice > 9){
                        handleInvalidInput();
                    }
                    else if(choice == 1){
                        addTabungan();
                        saveNodeTabungan();
                        break;
                    }
                    else if(choice == 2){
                        historiTabungan();
                        break;
                    }
                    else if(choice == 3){
                        tarikTabungan();
                        saveNodePenarikan();
                        break;
                    }
                    else if(choice == 4){
                        tampilanPenarikan();
                        break;
                    }
                    else if(choice == 5){
                        tampilkanSaldo();
                        break;
                    }
                    else if(choice == 6){
                        std::cout << endl;
                        break;
                    }
                }
            }
            else if(choice == 4){
                rataRataPemasukan();
                rataRataPengeluaran();
            }
            else if(choice == 5){
                break;
            }
            else{
                std::cout << "\033[1;31mInvalid Input! \033[0m" << endl << endl;
            }
        }
    }
    return 0;
}