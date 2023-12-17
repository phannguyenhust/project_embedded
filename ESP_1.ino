#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// khoi tao tham so cai dat cho man hinh
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#define SCREEN_WIDTH 128   // OLED display width, in pixels
#define SCREEN_HEIGHT 64   // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Cau hinh nut bam
const int btn_up = 27;
const int btn_down = 14;
const int btn_choose = 33;

//ket thuc cau hinh nut bam

//bat dau cau hinh cho RFID
#include <SPI.h>
#include <MFRC522.h>

 
#define RST_PIN         4          // Configurable, see typical pin layout above
#define SS_1_PIN        5         // Configurable, take a unused pin, only HIGH/LOW required, must be different to SS 2
#define SS_2_PIN        15       // Configurable, take a unused pin, only HIGH/LOW required, must be different to SS 1
#define SS_3_PIN        2
#define SS_4_PIN        32
#define SS_5_PIN        26

 
 #define NR_OF_READERS   5
 byte ssPins[] = {SS_1_PIN, SS_2_PIN, SS_3_PIN, SS_4_PIN, SS_5_PIN};
MFRC522 mfrc522[NR_OF_READERS];  // Create MFRC522 instance.

//ket thuc cau hinh cho RFID

//UID
String uidString = "";
//bien reject man hinh 8
bool check_get_pin_reject = true;

//bien nhan o de check rfid
char receive = 'h';

//cau hinh thong tin nguoi dung
String name_user = "";
String name_pin = "";
int moneyA = 6000;
int moneyB = 1500;
int cost1 = 2000;
int cost2 = 3000;

//bien trang thai doi nguoi dung lay pin
bool check_get_pin = true;
//bien du lieu nguoi dung lay pin ra
char value_check_get_pin = 'o';
// bien su dung de kiem soat trang thai cua man hinh
int status_monitor = 1;
//bien su dung de kiem soat trang thai nut bam o man hinh user
int status_btn_user = 1; // bang 1 la trang thai nut ben tren, bang 2 la trang thai nut phia duoi
// bien su dung de kiem soat trang thai nut bam o man hinh menu
int status_btn_menu = 1;
// bien su dung de kiem soat xem pin da duoc dua vao ben trong chua
// Bien kiem soat xem cua pin da duoc mo hay chua
bool door_pin_status = 0; // bien nay duoc cau thanh boi cac bien status_open_1 || status_open_2 || status_open_3 || status_open_4
// bien kiem soat pin da duoc dua vao hay chua khi moi ban dau
bool is_approve_put_pin = 0; // bang 0 tuc la chua, bang 1 la da duoc dua vao ben trong
// bien nay de luu xem RFID nao duoc mo
char receive_name_RFID = 'a';
void setup() {
  Serial.begin(9600);// cau hinh 
  // cau hinh uart
  Serial2.begin(9600);

  // Initialize with the I2C addr 0x3C (for the 128x32)
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  // Cau hinh cho 5 con RFID
  SPI.begin();    // Init SPI bus
  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN); // Init each MFRC522 card
    Serial.print(F("Reader "));
    Serial.print(reader);
    Serial.print(F(": "));
    mfrc522[reader].PCD_DumpVersionToSerial();
  }
  // ket thuc khoi tao RFID

  // khai bao nut bam
  pinMode(btn_up, INPUT_PULLDOWN); // phim 2 chan, 1 chan noi voi vcc, 1 chan noi voi tin hieu, khi nhan nut thi tin hieu chan do se len muc cao, the phai khai bao pull down
  pinMode(btn_down, INPUT_PULLDOWN);
  pinMode(btn_choose, INPUT_PULLDOWN);
  // ket thuc khai bao nut bam
}
  
// MAN HINH---------------------------------------------------------------------------------
void home_monitor(){
  display.clearDisplay();
  display.setTextSize(2); // cap do 1 la 7 pixel, cap do 2 la 14 pixel
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 10);
  display.println(F("WELL COME"));
  display.setTextSize(1);
  display.setCursor(10,40); 
  display.println(F("De su dung dich vu"));
  display.setCursor(5,50); 
  display.println(F("Dua the ATM ben phai"));
  // Show the display buffer on the screen
  display.display();
}
void user_monitor(String name_user, String name_pin){
  if (status_btn_user == 1){
    display.clearDisplay();
    display.setTextSize(2); // cap do 1 la 7 pixel, cap do 2 la 14 pixel
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 10);
    display.println(F("WELL COME"));
    display.setTextSize(1);
    display.setCursor(0,30); 
    display.println(name_user);
    display.setCursor(0,40); 
    display.println(name_pin);
    // phim chon chuc nang dich vao 60 pixel 
    display.setCursor(60, 40);
    display.println(F(">Tiep tuc"));
    display.setCursor(0, 50);
    display.print(F("TK: "));
    if(name_user == "Mr Huy"){
      display.print(moneyA);
    }
     if(name_user == "Mr Phan"){
      display.print(moneyB);
    }
    display.setCursor(60, 50);
    display.println(F("Thoat"));
    // Show the display buffer on the screen
    display.display();
  }
  if (status_btn_user == 2){
    display.clearDisplay();
    display.setTextSize(2); // cap do 1 la 7 pixel, cap do 2 la 14 pixel
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 10);
    display.println(F("WELL COME"));
    display.setTextSize(1);
    display.setCursor(0,30); 
    display.println(name_user);
    display.setCursor(0,40); 
    display.println(name_pin);
    // phim chon chuc nang dich vao 60 pixel 
    display.setCursor(60, 40);
    display.println(F("Tiep tuc"));
    display.setCursor(0, 50);
    display.print(F("TK: "));
    if(name_user == "Mr Huy"){
      display.print(moneyA);
    }
     if(name_user == "Mr Phan"){
      display.print(moneyB);
    }
    display.setCursor(60, 50);
    display.println(F(">Thoat"));
    // Show the display buffer on the screen
    display.display();
  }
}

// Man hinh lua chon loai pin va bang gia
void menu_Monitor(){
  if (status_btn_menu == 1){
    display.clearDisplay();
    display.setTextSize(2); // cap do 1 la 7 pixel, cap do 2 la 14 pixel
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(40, 0); // thut vao 40 pixel
    display.println(F("Menu"));
    display.setTextSize(1);
    display.setCursor(0,20); 
    display.print(F("Gia PIN loai A: "));
    display.println(cost1);
    display.setCursor(0,30); 
    display.print(F("Gia PIN loai B: "));
    display.println(cost2);
    display.setCursor(0,40); 
    // phim chon chuc nang dich vao 60 pixel 
    display.setCursor(60, 40);
    display.println(F(">Doi pin"));
    display.setCursor(60, 50);
    display.println(F("Thoat"));
    // Show the display buffer on the screen
    display.display();
  }
  if (status_btn_menu == 2){
    display.clearDisplay();
    display.setTextSize(2); // cap do 1 la 7 pixel, cap do 2 la 14 pixel
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(40, 0); // thut vao 40 pixel
    display.println(F("Menu"));
    display.setTextSize(1);
    display.setCursor(0,20); 
    display.print(F("Gia PIN loai A: "));
    display.println(cost1);
    display.setCursor(0,30); 
    display.print(F("Gia PIN loai B: "));
    display.println(cost2);
    display.setCursor(0,40); 
    // phim chon chuc nang dich vao 60 pixel 
    display.setCursor(60, 40);
    display.println(F("Doi pin"));
    display.setCursor(60, 50);
    display.println(F(">Thoat"));
    // Show the display buffer on the screen
    display.display();
  }
}
// Hien thi man hinh dat pin vao trong hop, neu chua co tin hieu dong y la da dat pin, bien is_approve_put_pin = 0, co tin hieu dong y, chuyen sang is_approve_put_pin = 1, chuyen man hinh
void put_Pin_Monitor(bool door_pin_status){
  if(door_pin_status == 1){
    display.clearDisplay();
    display.setTextSize(1); // cap do 1 la 7 pixel, cap do 2 la 14 pixel
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 20); // thut vao 40 pixel
    display.println(F("CUA PIN DA DUOC MO"));
    display.setCursor(10, 40); // thut vao 40 pixel
    display.println(F("DAT PIN CUA BAN VAO"));
    display.display();
  }
  else{
    display.clearDisplay();
    display.setTextSize(1); // cap do 1 la 7 pixel, cap do 2 la 14 pixel
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 20); // thut vao 40 pixel
    display.println(F("CHECKING CUA PIN"));
    display.setCursor(20, 40); // thut vao 40 pixel
    display.println(F("VUI LONG DOI"));
    delay(2000);
    display.display();
  }
}
// ham xac dinh check pin
void check_Pin_Monitor(){
     display.clearDisplay();
    display.setTextSize(1); // cap do 1 la 7 pixel, cap do 2 la 14 pixel
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 30); // thut vao 40 pixel
    display.println(F("DANG KIEM TRA PIN"));
    display.display();
}
// ham kiem tra pin hop le
void approve_pin_monitor(){
    display.clearDisplay();
    display.setTextSize(2); // cap do 1 la 7 pixel, cap do 2 la 14 pixel
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 10); // thut vao 40 pixel
    display.println(F("PIN HOP LE"));
     display.setTextSize(1);
    display.setCursor(20, 40); // thut vao 40 pixel
    display.println(F("DANG KIEM TRA"));
    display.setCursor(20, 50); // thut vao 40 pixel
    display.println(F("THANH TOAN"));
    display.display();
}
void reject_Pin_Monitor(){
  display.clearDisplay();
    display.setTextSize(2); // cap do 1 la 7 pixel, cap do 2 la 14 pixel
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 10); // thut vao 40 pixel
    display.println(F("TU CHOI PIN"));
    display.setTextSize(1);
    display.setCursor(10, 40); // thut vao 40 pixel
    display.println(F("DUA PIN RA KHOI ATM"));
    display.display();
}
// ham thanh toan hop le
void approve_payment_monitor(){
    display.clearDisplay();
    display.setTextSize(1); // cap do 1 la 7 pixel, cap do 2 la 14 pixel
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 30); // thut vao 40 pixel
    display.println(F("THANH TOAN THANH CONG"));
    display.setCursor(0, 40);
    display.print(F("TAI KHOAN CON "));
    if(name_user == "Mr Huy"){
      display.println(moneyA);
    }
     if(name_user == "Mr Phan"){
      display.println(moneyB);
    }
    display.display();
}
void reject_payment_monitor(){
  display.clearDisplay();
    display.setTextSize(1); // cap do 1 la 7 pixel, cap do 2 la 14 pixel
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 20); // thut vao 40 pixel
    display.println(F("THANH TOAN THAT BAI"));
    display.setCursor(10, 30);
    display.print(F("SO DU KHONG DU"));
    display.setCursor(0, 40);
    display.print(F("TAI KHOAN CON "));
     if(name_user == "Mr Huy"){
      display.println(moneyA);
    }
     if(name_user == "Mr Phan"){
      display.println(moneyB);
    }
    display.display();
}
void identify_user(){
  if(uidString == "224 133 139 31"){
    name_user = "Mr Huy";
    name_pin = "Loai A";
  }
  if(uidString == "227 202 61 15"){
    name_user = "Mr Phan";
    name_pin = "Loai A";
  }
}
void get_Pin_Monitor(){
  display.clearDisplay();
    display.setTextSize(1); // cap do 1 la 7 pixel, cap do 2 la 14 pixel
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(20, 20);
    display.println(F("CAM ON QUY KHACH"));
    display.setCursor(15, 30);
    display.println(F("DA SU DUNG DICH VU"));
    display.setCursor(30, 50); // thut vao 40 pixel
    display.println(F("HAY LAY PIN"));
    display.display();
}

//man hinh kiem tra thanh toan
void checking_payment_monitor(){
  display.clearDisplay();
    display.setTextSize(1); // cap do 1 la 7 pixel, cap do 2 la 14 pixel
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 20);
    display.println(F("CHECKING THANH TOAN"));
    display.display();
}
// KET THUC MAN HINH ---------------------------------------------------------------------------------------
// bien trang thai xac dinh co nguoi trong database
bool is_customer = false;

//ham de xy ly RFID nguoi dung
// bien nay de xac dinh id cua nguoi dung
 // Tạo một biến kiểu chuỗi và gán giá trị từ mảng UID
// bien nay kiem soat xem the do duoc chap nhan khong
int is_approve_pin = 0;// 2 la pin hop le va du tien, 1 ; la pin khong hop le
void RFID_handler(int reader){
  if(mfrc522[reader].PICC_IsNewCardPresent()){ // neu phat hien the moi
      if(mfrc522[reader].PICC_ReadCardSerial()){// neu doc duoc ma so cua the
        // Khai báo một mảng để lưu trữ UID
      byte UID[10];

      // Lấy UID và lưu vào mảng UID
      for (byte i = 0; i < mfrc522[reader].uid.size; i++) 
      { 
        UID[i] = mfrc522[reader].uid.uidByte[i];
      }
      for (byte i = 0; i < mfrc522[reader].uid.size; i++) 
      { 
        uidString += String(UID[i]);
      if (i < mfrc522[reader].uid.size - 1) {
        uidString += " ";  // Thêm khoảng trắng nếu không phải là phần tử cuối cùng
        }
      }
      // In ra Serial Monitor chuỗi UID
        Serial.print("UID của thẻ: ");
        Serial.print(reader);
        Serial.print(" ");
        Serial.println(uidString);
        // Tiếp tục xử lý hoặc kết thúc tương tác với thẻ
        mfrc522[reader].PICC_HaltA();  
        mfrc522[reader].PCD_StopCrypto1();
      }
    } 
    Serial.print("Chuan bi in ");
    Serial.println(uidString);
    if(reader == 0 && (uidString == "224 133 139 31" || uidString == "227 202 61 15")){
      identify_user();
      status_monitor =2;
    }
    //tam thoi, xu ly cho man hinh check pin, de dua pin vao man hinh tiep theo
    if((reader == 2 || reader == 1 ) && (uidString == "227 232 67 173" || uidString == "99 125 95 47" ) && status_monitor == 6){
      // if(moneyA >= cost1){
      //   moneyA -= cost1;
        // status_monitor = 7;
      // truyen 2 tuc la dong y pin hop le va du tien
      is_approve_pin = 2;
      // char cal = is_approve_pin + '0';
      // // dong y thi gui tin hieu 2 sang ben kia
      // Serial2.write(cal);
      //1
     // }
      // else if(moneyA < cost1){
      //   status_monitor = 8;
      //   // truyen 2 tuc la dong y pin hop le va du tien
      //   is_approve_pin = 1;
      //   char cal = is_approve_pin + '0';
      //   // dong y thi gui tin hieu 2 sang ben kia
      //   Serial2.write(cal);
      // }
    }
    else if((reader == 2 || reader == 1 ) && (uidString == "193 26 203 36" || uidString == "243 39 91 189" ) && status_monitor == 6){
      // status_monitor = 8;
      // truyen 2 tuc la dong y pin hop le va du tien
      is_approve_pin = 1;
      // char cal = is_approve_pin + '0';
      // // dong y thi gui tin hieu 2 sang ben kia
      // Serial2.write(cal);
    } 
    // else if((reader == 3 || reader == 4 ) && (uidString == "193 26 203 36" || uidString == "243 39 91 189" ) && status_monitor == 6){
    //   if(moneyB >= cost2){
    //     status_monitor = 7;
    //     moneyB -= cost2;
    //     // truyen 2 tuc la dong y pin hop le va du tien
    //     is_approve_pin = 2;
    //     char cal = is_approve_pin + '0';
    //     // dong y thi gui tin hieu 2 sang ben kia
    //     Serial2.write(cal);
    //   }
    //   else if(moneyB < cost2){
    //     // status_monitor = 8;
    //     // truyen 2 tuc la dong y pin hop le va du tien
    //     is_approve_pin = 1;
    //     char cal = is_approve_pin + '0';
    //     // dong y thi gui tin hieu 2 sang ben kia
    //     Serial2.write(cal);
    //   }
      
    // } 
    // else if((reader == 3 || reader == 4 ) && (uidString == "227 232 67 173" || uidString == "99 125 95 47" ) && status_monitor == 6){
    //   // dong phia duoi da chuyen roi
    //   // status_monitor = 8;
    //   // truyen 2 tuc la dong y pin hop le va du tien
    //   is_approve_pin = 1;
    //   char cal = is_approve_pin + '0';
    //   // dong y thi gui tin hieu 2 sang ben kia
    //   Serial2.write(cal);
    // }
    uidString = "";// de buoc sau phai nhap lai the
}
// ket thuc ham xu ly RFID nguoi dung

// ham xu ly nut bam man hinh user
void btn_user_monitor_hander(){
  int status_btn_up = digitalRead(btn_up);
    int status_btn_down = digitalRead(btn_down);
    int status_btn_choose = digitalRead(btn_choose);
    if (status_btn_up == HIGH) {
      Serial.println(status_btn_up);
      Serial.print("up");
      status_btn_user =1;
      delay(500);
    }
    // kiem tra nut down duoc nhan
    if (status_btn_down == HIGH) {
      Serial.println(status_btn_down);
      Serial.print("down");
      status_btn_user =2;
      delay(500);
    }
    if(status_btn_choose== HIGH && status_btn_user == 1){
        status_monitor = 3;
    }
    // neu nut chon duoc nhan va lua chon nguoi dung dang la 2
    if(status_btn_choose== HIGH && status_btn_user == 2){
        status_monitor = 1;
    }
}
// ket thuc ham xu ly nut bam man hinh user
// ham xu ly nut bam man hinh menu
void btn_menu_monitor_hander(){
    int status_btn_up = digitalRead(btn_up);
    int status_btn_down = digitalRead(btn_down);
    int status_btn_choose = digitalRead(btn_choose);
    if (status_btn_up == HIGH) {
      Serial.println(status_btn_up);
      Serial.print("up");
      status_btn_menu = 1;
      delay(500);
    }
    // kiem tra nut down duoc nhan
    if (status_btn_down == HIGH) {
      Serial.println(status_btn_down);
      Serial.print("down");
      status_btn_menu = 2;
      delay(500);
    }
    if(status_btn_choose== HIGH && status_btn_menu == 1){
      // gui tin hieu dieu khien sang cho ben kia la bat dau chay quy trinh ben ho
      if(name_pin == "Loai A"){
        Serial2.write('1');
        Serial.println("da gui tin hieu 1 de kich hoat");
        signal_number_box_check_RFID();
        Serial.println(receive_name_RFID);
        Serial.println("da gui tin hieu 1 de kich hoat");
      }
      if(name_pin == "Loai B"){
        Serial2.write('2');
        Serial.println("da gui tin hieu 2 de kich hoat");
        signal_number_box_check_RFID();
        Serial.println(receive_name_RFID);
        Serial.println("da gui tin hieu 2 de kich hoat");
      }
      
    }
    // neu nut chon duoc nhan va lua chon nguoi dung dang la 2
    if(status_btn_choose== HIGH && status_btn_menu == 2){
        status_monitor = 2;
    }
}
// ket thuc ham xu ly nut bam man hinh menu

// ham xu ly doi tin hieu o nao duoc mo trong RFID

// bien nay de dieu khien ham duoi
bool wait_receive = true;
void signal_number_box_check_RFID(){
   while(wait_receive){
        if (Serial2.available() > 0) {
        receive_name_RFID = (char)Serial2.read();
        Serial.println(receive_name_RFID);
      }
      if(receive_name_RFID == '1'){
        status_monitor = 4;
        wait_receive = false;        
      }
  }
}
// bien nay de kiem soat man hinh so 4
// man hinh so 4 yeu cau dua pin vao ben trong
bool status_check_signal_put_pin = true;
// bien kiem soat trang thai dat lai man hinh
bool status_is_new_cycle = false;
void loop() {
  // Set text size, color, and print "Hello" at coordinates (0, 0)
  if (status_monitor == 1){
    home_monitor();
    delay(1000);
    // xu ly RFID tuc la lay id tu the (La mot chuoi so) de so sanh voi firebase
    RFID_handler(0);
  }
    // ham goi toi cai quet the
    //if quet the va co database thi chuyen sang man hinh 2, status_monitor =2, 
  // lay du lieu tu filebase
  // xu ly man hinh user
  if (status_monitor == 2){
    user_monitor(name_user, name_pin);
    delay(1000);
    // Goi den ham xu ly nut bam o man hinh user
    btn_user_monitor_hander();
  }
  if (status_monitor == 3){
    menu_Monitor();
    delay(1000);
    // goi den ham xu ly nut bam o man hinh menu monitor
    btn_menu_monitor_hander();
  }

  if (status_monitor == 4){
    put_Pin_Monitor(1);
    delay(1000);
    while(status_check_signal_put_pin){
      if (Serial2.available() > 0) {
      char receiveTwo = (char)Serial2.read();
      receive = receiveTwo;
      Serial.println(receive);
    }
      if(receive == '2' || receive == '1'){
        delay(1000);
        status_check_signal_put_pin = false;
        status_monitor = 5;
      }
      else if(receive == '3'){
        status_check_signal_put_pin = false;
        status_is_new_cycle = true;
        status_monitor = 1;
      }
    }
    Serial.println("Man hinh so 5");
  }
  if(status_monitor == 5){
    // hien thi man hinh kiem tra pin
    check_Pin_Monitor();
    // trang thai nay chi de hien thi man hinh
    status_monitor = 6;
    Serial.println("Man hinh so 6");
  }
  if(status_monitor == 6){
    // ham goi toi lang nghe xem ben esp 2 gui ve o so may dang trong
    Serial.println("bat dau wait");
    delay(1000); 
    Serial.println("ket thuc wait");
    //chuyen ky tu ten cua box can check sang int
    int box_number = receive-'0';
    Serial.println(box_number);
    //ham mo RFID tuong ung voi o do de doc pin
    RFID_handler(box_number); // hien tai dang gia dinh la thang so 1 dang trong, goi RFID quet ham so 1
    // trong ham RFID nay thi no da thong bao la neu pin hop le thi sang man hinh 7
    // neu pin khong hop le sang man hinh 8
    //goi den bien is_approve_pin, de quyet dinh chuyen man hinh nao
    // 1 tuc la tu choi pin, man 8 man hinh sai pin
    if (is_approve_pin == 1){
      status_monitor = 8;
    }
// 2 la dong y pin    , man 7 man hinh dung pin
    if(is_approve_pin == 2){
      status_monitor = 7; // man hinh 7 la man hinh dong y pin
    }
  }
  // bat dau sua 14h40 12/15/2023
  // sau doan nay se thong bao man hinh kiem tra pin hop le
  if(status_monitor == 7){
    
    // hien thi man hinh chap nhan pin
    approve_pin_monitor();
    delay(3000);
    // tiep theo la sang man hinh kiem tra thanh toan
    checking_payment_monitor();
    delay(3000);
    // bat dau xu ly thanh toan
    // neu chap nhan thanh toan thi ra man hinh 9, khong chap nhan thanh toan quay lai man hinh 8
    // man hinh 8 vua la man hinh tu choi pin, vua la man hinh hien ra sau khi tu choi thanh toan
    // CODE TIEP O DAY
    // xu ly thanh 
    if (name_user == "Mr Huy" && name_pin =="Loai A"){
      if(moneyA >= cost1){
          moneyA = moneyA - cost1;
          status_monitor = 9;
          // du tien thi bat dau gui tin hieu cho ben kia mo pin moi cho nguoi dung lay pin moi
          Serial2.write('2');
      }
      else{
          status_monitor = 10;
          // Serial2.write('1');
      }
    }
    // xu ly thanh toan  cho nguoi dung thu hai la chap nhan pin nhung khong du tien
    if (name_user == "Mr Phan" && name_pin =="Loai A"){
      if(moneyB < cost1){
          status_monitor = 10;
          // khong du tien thi bat dau gui tin hieu cho ben kia mo pin moi cho nguoi dung lay pin cu ra
          // Serial2.write('1');
      }
      else{
          moneyB = moneyB - cost1;
          status_monitor = 9;
      }
    }
  }
  // day la phan danh cho chap nhan thanh toan va chap nhan pin
  if(status_monitor == 9){
    approve_payment_monitor();
    delay(3000);
    get_Pin_Monitor();
    delay(3000);
    while(check_get_pin){
      Serial.println("dang cho tin hieu dong");
      if (Serial2.available() > 0) {
        char receiveTwo = (char)Serial2.read();
        value_check_get_pin = receiveTwo;
        Serial.println(value_check_get_pin);
      }
      if(value_check_get_pin == '9' || value_check_get_pin == '3'){
        check_get_pin = false;
        status_monitor = 1;
        // dat lai du lieu
        status_is_new_cycle = true;
      } 
    }
  }
  if(status_monitor == 8){
    // hien thi man hinh tu choi pin
    
    reject_Pin_Monitor();
    delay(2000);
    Serial2.write('1');
    // bat dau them 15/12
    get_Pin_Monitor();
    //ket thuc bat dau them 15/12
    while(check_get_pin_reject){
      Serial.println("dang cho tin hieu dong");
      if (Serial2.available() > 0) {
        char receiveTwo = (char)Serial2.read();
        value_check_get_pin = receiveTwo;
        Serial.println(value_check_get_pin);
      }
      if(value_check_get_pin == '9' || value_check_get_pin == '3'){
        check_get_pin_reject = false;
        status_monitor = 1;
        // dat lai du lieu
        status_is_new_cycle = true;
      } 
    }
  }
  if(status_monitor == 10){
    // hien thi man hinh tu choi pin
    reject_payment_monitor();
    Serial2.write('1');
    delay(2000);
    // bat dau them 15/12
    get_Pin_Monitor();
    //ket thuc bat dau them 15/12
    while(check_get_pin_reject){
      Serial.println("dang cho tin hieu dong");
      if (Serial2.available() > 0) {
        char receiveTwo = (char)Serial2.read();
        value_check_get_pin = receiveTwo;
        Serial.println(value_check_get_pin);
      }
      if(value_check_get_pin == '9' || value_check_get_pin == '3'){
        check_get_pin_reject = false;
        status_monitor = 1;
        // dat lai du lieu
        status_is_new_cycle = true;
      } 
    }
  }
  // xu ly bien de quay lai ban dau
  if(status_is_new_cycle){
    receive = 'h';
    name_user = "";
    name_pin = "";
    check_get_pin = true;
    value_check_get_pin = 'o';
    status_monitor = 1;
    status_btn_user = 1;
    status_btn_menu = 1;
    door_pin_status = 0;
    is_approve_put_pin = 0;
    receive_name_RFID = 'a';
    is_customer = false;
    is_approve_pin = 0;
    wait_receive = true;
    status_check_signal_put_pin = true;
    status_is_new_cycle =false;
    check_get_pin_reject = true;
  }
}