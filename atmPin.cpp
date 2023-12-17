#include <systemc.h>
using namespace std;

SC_MODULE(ATMPIN) {
  //Khai báo biến chứa tiền của các khách hàng A, B, C, D
    int moneyA;
    int moneyB;
    int moneyC;
    int moneyD;
    int money;
  // Khai báo biến Random khách hàng tới đổi trả pin
    int randomCustomer;
  // Khai báo biến Random Pin khách hàng tới đổi trả
    int randomBattery;
  // Taọ mảng giả lập chứa tín hiệu khay pin trống của mỗi hàng
    int pinHonda[2]={1,0};
    int pinVinFast[2]={1,0};
  
  // Biến lưu trạng thái hộp pin trống và hộp chứa pin đổi
    int emptyBatteryHonda;
    int emptyBatteryVinFast;
    int containBatteryHonda;
    int containBatteryVinFast;
    
    sc_event thread1_done_event; // Sự kiện để báo hiệu Thread 1 đã hoàn thành
    sc_event thread2_done_event; // Sự kiện để báo hiệu Thread 2 đã hoàn thành
    sc_event thread3_done_event; // Sự kiện để báo hiệu Thread 3 đã hoàn thành
    sc_event thread4_done_event; // Sự kiện để báo hiệu Thread 4 đã hoàn thành
    sc_event thread5_done_event; // Sự kiện để báo hiệu Thread 5 đã hoàn thành

    bool run_thread2; // Biến đánh dấu xem Thread 2 có thể chạy hay không
    bool run_thread3; // Biến đánh dấu xem Thread 3 có thể chạy hay không
    bool run_thread4;
    bool run_thread5;

  // Khai báo biến lưu tên Khách hàng, tên pin, tiền khách hàng
    char nameCustomer;
    string nameBattery;
    int moneyCustomer;

  // Khai báo biến lưu tín hiệu của cảm biến siêu âm
    int ultrasonicSensors;

  // Khai báo biến check trạng thái True or False
    bool checkEnoughMoney;

  // Khai báo biến trung gian
    int temp;

  // Khai báo biến đếm
    int dem;
  // Khai báo biến kiểm tra vị trí pin đã vào vị trí đúng theo quy định 
    bool checkPositionBattery;
    SC_CTOR(ATMPIN){
        run_thread2 = false;
        run_thread3 = false;
        run_thread4 = false;
        run_thread5 = false;
        srand(time(NULL));  // Khởi tạo seed cho hàm rand()

        // moneyA = 10;
        // moneyB = 10;
        // moneyC = 10;
        // moneyD = 10;
        moneyB = (rand() % 16 + 5) * 10;
        moneyC = (rand() % 16 + 5) * 10;
        moneyD = (rand() % 16 + 5) * 10;
        moneyA = (rand() % 16 + 5) * 10;
        cout<<endl << "MÔ PHỎNG CHƯƠNG TRÌNH ATM ĐỔI PIN" << endl;
        SC_THREAD(thread1);
        SC_THREAD(thread2);
        SC_THREAD(thread3);
        SC_THREAD(thread4);
        SC_THREAD(thread5);
        // dont_initialize();

        // sensitive << thread1_done_event; // Thread 2 và 3 chờ sự kiện từ Thread 1
    }

    void thread1() {
        // THÔNG BÁO RA MÀN HÌNH KHÁCH HÀNG ĐỔI PIN
            wait(SC_ZERO_TIME);
                while (true) {
                    cout<<endl<<"-----------------------------"<<endl;
                    run_thread2 = false;
                    run_thread3 = false;
                    run_thread4 = false;
                    run_thread5 = false;
                    randomCustomer = rand() % 4 + 1; // Random từ 1 đến 4
                    if(randomCustomer == 1){
                        nameCustomer = 'A';
                        nameBattery = "Honda";
                        moneyCustomer = moneyA;
                        cout<<"Custom "<<nameCustomer<<" is changing the Battery!"<<endl;
                    }else if(randomCustomer == 2){
                        nameCustomer = 'B';
                        nameBattery = "Honda";
                        moneyCustomer = moneyB;
                        cout<<"Custom "<<nameCustomer<<" is changing the Battery!"<<endl;
                    }else if(randomCustomer == 3){
                        nameCustomer = 'C';
                        nameBattery = "VinFast";
                        moneyCustomer = moneyC;
                        cout<<"Custom "<<nameCustomer<<" is changing the Battery!"<<endl;
                    }else if(randomCustomer == 4){
                        nameCustomer = 'D';
                        nameBattery = "VinFast";
                        moneyCustomer = moneyD;
                        cout<<"Custom "<<nameCustomer<<" is changing the Battery!"<<endl; 
                    }
                    cout<<"-----------------------------"<<endl;
                    run_thread2 = true;
                    thread1_done_event.notify();
                wait(60,SC_SEC);
            }
    }

    void thread2() {
        // HIỂN THỊ THÔNG TIN KHÁCH HÀNG
        while(true){
                // Chờ sự kiện từ Thread 2
                wait(thread1_done_event);
                // HIỂN THỊ THÔNG TIN KHÁCH HÀNG ĐỔI PIN
                cout<<"|THÔNG TIN KHÁCH HÀNG| tại "<<sc_time_stamp()<<endl;
                cout<<"1. Tên           : Khách hàng "<<nameCustomer<<endl;
                cout<<"2. Tiền tài khoản: "<<moneyCustomer<<endl;
                cout<<"3. Loại pin      : "<<nameBattery<<endl;
                wait(1, SC_SEC);
                cout<<"Chờ khách hàng ấn Tiếp tục! tại "<<sc_time_stamp()<<endl;
                wait(1,SC_SEC);
                cout<<"Chờ khách hàng ấn Đổi pin! tại "<<sc_time_stamp()<<endl; 
                cout<<"-----------------------------"<<endl;
                run_thread3 = true;
                thread2_done_event.notify();
            }
        }

        void thread3() {
            // KIỂM TRA THÔNG TIN PIN
            while(true){
                // Chờ sự kiện từ Thread 2
                wait(thread2_done_event);
                wait(3,SC_SEC);
                // KIỂM TRA HỘP PIN TRỐNG THEO ĐÚNG LOẠI 
                if(nameBattery == "Honda" & pinHonda[0] == 1 & pinHonda[1] ==0){
                    emptyBatteryHonda = 2;
                    containBatteryHonda = 1;
                }else{
                    emptyBatteryHonda = 1;
                    containBatteryHonda = 2;         
                }

                if(nameBattery == "VinFast" & pinVinFast[0] == 1 & pinVinFast[1] == 0){
                    emptyBatteryVinFast = 2;
                    containBatteryVinFast = 1;
                }else{
                    emptyBatteryVinFast = 1;
                    containBatteryVinFast = 2;
                }
                // KIỂM TRA THÔNG TIN PIN KHÁCH HÀNG ĐƯA VÀO
                cout<<"|KIỂM TRA THÔNG TIN PIN ĐƯA VÀO| tại "<<sc_time_stamp()<<endl;
                if(nameBattery == "Honda"){
                    wait(3,SC_SEC);
                    cout<<endl<<"Bước 1: Vui lòng đặt pin vào hộp số "<<emptyBatteryHonda<<" hàng pin Honda đang được mở."<<endl;
                    cout<<"Hộp pin đã được mở tại "<<sc_time_stamp()<<endl;
                }else{
                    wait(3,SC_SEC);
                    cout<<endl<<"Bước 1: Vui lòng đặt pin vào hộp số "<<emptyBatteryVinFast<<" hàng pin VinFast đang được mở."<<endl;
                    cout<<"Hộp pin đã được mở tại "<<sc_time_stamp()<<endl;
                }
                //Kiểm tra pin được đặt đúng vị trí hay không, nếu đúng thì chuyển bước tiếp, nếu sai và quá 10s thì hộp sẽ tự đóng, kết thúc quá trình
                wait(2,SC_SEC);
                checkPositionBattery = true;

                for(int i = 0;i<5;i++){
                    ultrasonicSensors = rand()% 2;
                    if(ultrasonicSensors == 0){
                        checkPositionBattery = false;
                        cout<<"Pin đã đặt sai tại "<<sc_time_stamp()<<endl;
                        if(i==4){
                            break;
                        }
                    }else{
                        cout<<"Pin đã đặt đúng tại "<<sc_time_stamp()<<endl;
                        checkPositionBattery = true;
                        break;
                    }
                    wait(2,SC_SEC);
                }

                if(checkPositionBattery == false){
                    wait(3,SC_SEC);
                    cout<<endl<<"THÔNG BÁO: Khách hàng đặt pin không đúng, hộp pin đang đóng lại"<<endl;
                    cout<<"Hộp pin đóng lại tại "<<sc_time_stamp()<<endl;
                    cout<<"-----------------------------"<<endl;
                    cout<<"Kết thúc quá trình tại "<<sc_time_stamp()<<endl;
                    cout<<"-----------------------------"<<endl;
                }else{
                    cout<<endl<<"Bước 2: Vui lòng bỏ tay ra khỏi hộp, hộp pin sẽ được đóng lại trong giây lát."<<endl;
                    wait(5,SC_SEC);
                    cout<<"Pin đã được đưa vào, hộp pin đóng lại tại "<<sc_time_stamp()<<endl;
                    cout<<endl<<"Bước 3: Hộp pin đã được đóng. Hệ thống đang tiến hành xác thực."<<endl;
                    randomBattery = rand() % 4 + 1;

                    if(randomBattery == 1 & randomCustomer == 1){
                        wait(2,SC_SEC);
                        cout<<endl<<"THÔNG BÁO: Xác thực thông tin pin THÀNH CÔNG"<<endl;      
                        cout<<"Xác thực THÀNH CÔNG tại "<<sc_time_stamp()<<endl;          
                        run_thread4 = true;
                    }else if(randomBattery == 2 & randomCustomer == 2){
                        wait(2,SC_SEC);
                        cout<<endl<<"THÔNG BÁO: Xác thực thông tin pin THÀNH CÔNG"<<endl;  
                        cout<<"Xác thực THÀNH CÔNG tại "<<sc_time_stamp()<<endl;              
                        run_thread4 = true;
                    }else if(randomBattery == 3 & randomCustomer == 3){
                        wait(2,SC_SEC);
                        cout<<endl<<"THÔNG BÁO: Xác thực thông tin pin THÀNH CÔNG"<<endl;                
                        cout<<"Xác thực THÀNH CÔNG tại "<<sc_time_stamp()<<endl;
                        run_thread4 = true;
                    }else if(randomBattery == 4 & randomCustomer == 4){
                        wait(2,SC_SEC);
                        cout<<endl<<"THÔNG BÁO: Xác thực thông tin pin THÀNH CÔNG"<<endl;  
                        cout<<"Xác thực THÀNH CÔNG tại "<<sc_time_stamp()<<endl;              
                        run_thread4 = true;
                    }else{
                        wait(2,SC_SEC);
                        cout<<endl<<"THÔNG BÁO: Xác thực thông tin THẤT BẠI, sai thông tin pin!"<<endl;
                        cout<<"Xác thực THẤT BẠI tại "<<sc_time_stamp()<<endl;
                        if(nameBattery == "Honda"){
                            wait(3,SC_SEC);
                            // cout<<"Lấy pin ra khỏi hộp tại "<<sc_time_stamp()<<endl;
                            cout<<endl<<"THÔNG BÁO: Hộp pin số "<<emptyBatteryHonda<<" hàng Honda đang mở, vui lòng lấy pin cũ ra!"<<endl;
                            cout<<"Hộp pin đang mở tại "<<sc_time_stamp()<<endl;
                        }else{
                            wait(3,SC_SEC);
                            // cout<<"Lấy pin ra khỏi hộp tại "<<sc_time_stamp()<<endl;
                            cout<<endl<<"THÔNG BÁO: Hộp pin số "<<emptyBatteryVinFast<<" hàng VinFast đang mở, vui lòng lấy pin cũ ra!"<<endl;
                            cout<<"Hộp pin đang mở tại "<<sc_time_stamp()<<endl;
                        }
                        wait(2,SC_SEC);
                        checkPositionBattery = true;

                        for(int i = 0;i<5;i++){
                            ultrasonicSensors = rand()% 2;
                            if(ultrasonicSensors == 1){
                                checkPositionBattery = false;
                                cout<<"Pin chưa được lấy ra khỏi hộp tại "<<sc_time_stamp()<<endl;
                                if(i==4){
                                    break;
                                }
                            }else{
                                cout<<"Pin đã được lấy ra khỏi hộp tại "<<sc_time_stamp()<<endl;
                                checkPositionBattery = true;
                                break;
                            }
                            wait(2,SC_SEC);
                        }

                        if(checkPositionBattery == false){
                            wait(3,SC_SEC);
                            cout<<endl<<"THÔNG BÁO: Hộp pin đang đóng lại, pin của bạn đã bị thu hồi, vui lòng liên hệ với CSKH"<<endl;
                            cout<<"Pin đã bị thu hồi tại "<<sc_time_stamp()<<endl;
                            cout<<"-----------------------------"<<endl;
                            cout<<"Kết thúc quá trình tại "<<sc_time_stamp()<<endl;
                            cout<<"-----------------------------"<<endl;
                        }else{
                            wait(3, SC_SEC);
                            // cout<<"Lấy pin THÀNH CÔNG tại "<<sc_time_stamp()<<endl;
                            cout<<endl<<"THÔNG BÁO: Lấy pin THÀNH CÔNG, hộp pin đã được đóng lại"<<endl;
                            cout<<"Hộp pin đã được đóng lại tại "<<sc_time_stamp()<<endl;
                            cout<<"-----------------------------"<<endl;
                            cout<<"Kết thúc qúa trình tại "<<sc_time_stamp()<<endl;
                            cout<<"-----------------------------"<<endl;
                        }
                    }
                }
                thread3_done_event.notify();   
            }
    }

    void thread4(){
        // HIỂN THỊ THÔNG TIN THANH TOÁN
        while(true){
            wait(thread3_done_event);
            if(run_thread4){
                wait(3,SC_SEC);
                cout<<"|HIỂN THỊ THÔNG TIN THANH TOÁN| tại "<<sc_time_stamp()<<endl<<endl;
                cout<<"1. Loại Pin: "<<nameBattery<<endl;
                if(nameBattery == "Honda"){
                    money = 50;
                }else{
                    money = 60;
                }
                cout<<"2. Giá tiền: "<<money<<endl;
                checkEnoughMoney = true;
                if(nameCustomer == 'A' & moneyA < 50){
                    checkEnoughMoney = false;
                }else if(nameCustomer == 'B' & moneyB < 50){
                    checkEnoughMoney = false;
                }else if(nameCustomer == 'C' & moneyC < 60){
                    checkEnoughMoney = false;
                }else if(nameCustomer == 'D' & moneyD < 60){
                    checkEnoughMoney = false;
                }

                if(checkEnoughMoney == false){
                    cout<<endl<<"THÔNG BÁO: Tài khoản không đủ để thành toán, vui lòng nạp thêm!"<<endl;
                    cout<<"Thanh toán THẤT BẠI tại "<<sc_time_stamp()<<endl;
                    if(nameBattery == "Honda"){
                        wait(3,SC_SEC);
                        cout<<"Hộp pin số "<<emptyBatteryHonda<<" hãng Honda đang mở, vui lòng lấy pin ra!"<<endl;
                        cout<<"Hộp pin đã mở tại "<<sc_time_stamp()<<endl;
                    }else{
                        wait(3,SC_SEC);
                        cout<<"Hộp pin số "<<emptyBatteryVinFast<<" hãng VinFast đang mở, vui lòng lấy pin ra!"<<endl;
                        cout<<"Hộp pin đã mở tại "<<sc_time_stamp()<<endl;
                    }
                    wait(2,SC_SEC);
                    checkPositionBattery = true;
                    
                    for(int i = 0;i<5;i++){
                        ultrasonicSensors = rand()% 2;
                        if(ultrasonicSensors == 1){
                            checkPositionBattery = false;
                            cout<<"Pin chưa được lấy ra khỏi hộp tại "<<sc_time_stamp()<<endl;
                            if(i==4){
                                break;
                            }
                        }else{
                            cout<<"Pin đã được lấy ra khỏi hộp tại "<<sc_time_stamp()<<endl;
                            checkPositionBattery = true;
                            break;
                        }
                        wait(2,SC_SEC);
                    }

                    if(checkPositionBattery == false){
                        wait(3,SC_SEC);
                        cout<<endl<<"THÔNG BÁO: Hộp pin đang đóng lại, pin của bạn đã bị thu hồi, vui lòng liên hệ CSKH"<<endl;
                        cout<<"Pin đã bị thu hồi tại "<<sc_time_stamp()<<endl;
                        cout<<"-----------------------------"<<endl;
                        cout<<"Kết thúc quá trình tại "<<sc_time_stamp()<<endl;   
                        cout<<"-----------------------------"<<endl;
                    }else{
                        wait(3,SC_SEC);
                        cout<<endl<<"THÔNG BÁO: Lấy pin cũ THÀNH CÔNG"<<endl;
                        cout<<"Lấy pin cũ THÀNH CÔNG tại "<<sc_time_stamp()<<endl;
                        cout<<"-----------------------------"<<endl;
                        cout<<"Kết thúc quá trình tại "<<sc_time_stamp()<<endl;   
                        cout<<"-----------------------------"<<endl;
                    }
                }else{
                    cout<<endl<<"THÔNG BÁO: Thanh toán THÀNH CÔNG."<<endl;
                    cout<<"Thanh toán THÀNH CÔNG tại "<<sc_time_stamp()<<endl;
                    if(nameCustomer == 'A'){
                        moneyA -= 50;
                        cout<<"Tài khoản khách hàng còn lại: "<<moneyA<<endl;
                        if(moneyA < 50){
                            cout<<endl<<"CẢNH BÁO: Tài khoản của khách hàng đã hết, vui lòng nạp thêm để không ảnh hưởng đến lần đổi pin tiếp theo. Trân trọng!"<<endl;
                        }
                        run_thread5 = true;
                    }else if(nameCustomer == 'B'){
                        moneyB -=50;
                        cout<<"Tài khoản khách hàng còn lại: "<<moneyB<<endl;
                        if(moneyB < 50){
                            cout<<endl<<"CẢNH BÁO: Tài khoản của khách hàng đã hết, vui lòng nạp thêm để không ảnh hưởng đến lần đổi pin tiếp theo. Trân trọng!"<<endl;
                        }
                        run_thread5 = true;
                    }else if(nameCustomer == 'C'){
                        moneyC -=60;
                        cout<<"Tài khoản khách hàng còn lại: "<<moneyC<<endl;
                        if(moneyC < 60){
                            cout<<endl<<"CẢNH BÁO: Tài khoản của khách hàng đã hết, vui lòng nạp thêm để không ảnh hưởng đến lần đổi pin tiếp theo. Trân trọng!"<<endl;
                        }           
                        run_thread5 = true;   
                    }else if(nameCustomer == 'D'){
                        moneyD -=60;
                        cout<<"Tài khoản khách hàng còn lại: "<<moneyD<<endl;
                        if(moneyD < 60){
                            cout<<endl<<"CẢNH BÁO: Tài khoản của khách hàng đã hết, vui lòng nạp thêm để không ảnh hưởng đến lần đổi pin tiếp theo. Trân trọng!"<<endl;
                        }    
                        run_thread5 = true;
                    }              
                    cout<<"-----------------------------"<<endl;
                }
            }
            thread4_done_event.notify();
        }
    }
    void thread5(){
        // ĐỔI PIN 
        while(true){
            wait(thread4_done_event);
            if(run_thread5){
                wait(3,SC_SEC);
                cout<<"|ĐỔI PIN MỚI CHO KHÁCH HÀNG| tại "<<sc_time_stamp()<<endl;
                if(nameBattery == "Honda"){
                    wait(3,SC_SEC);
                    cout<<endl<<"Bước 1: Khay pin số "<<containBatteryHonda<<" hàng pin Honda đang được mở, vui lòng lấy pin ra khỏi khay "<<endl;
                    cout<<"Hộp đã được mở tại "<<sc_time_stamp()<<endl;
                }else{
                    wait(3,SC_SEC);
                    cout<<endl<<"Bước 1: Khay pin số "<<containBatteryVinFast<<" hàng pin Honda đang được mở, vui lòng lấy pin ra khỏi khay "<<endl;
                    cout<<"Hộp đã được mở tại "<<sc_time_stamp()<<endl;
                }
                wait(2,SC_SEC);
                checkPositionBattery = true;

                for(int i = 0;i<5;i++){
                    ultrasonicSensors = rand()% 2;
                    if(ultrasonicSensors == 1){
                        checkPositionBattery = false;
                        cout<<"Pin chưa được lấy ra khỏi hộp tại "<<sc_time_stamp()<<endl;
                        if(i==4){
                            break;
                        }
                    }else{
                        cout<<"Pin đã được lấy ra khỏi hộp tại "<<sc_time_stamp()<<endl;
                        checkPositionBattery = true;
                        break;
                    }
                    wait(2,SC_SEC);
                }

                if(checkPositionBattery == false){
                    wait(3,SC_SEC);
                    cout<<endl<<"THÔNG BÁO: Hộp pin mới đang được đóng lại, pin cũ của bạn đã bị thu hồi, vui lòng liên hệ CSKH"<<endl;
                    cout<<"Pin cũ đã bị thu hồi tại "<<sc_time_stamp()<<endl;
                }else{
                    wait(3,SC_SEC);
                    cout<<endl<<"Bước 2: Lấy pin THÀNH CÔNG, hộp pin đang được đóng"<<endl;
                    cout<<"Hộp đã được đóng lại tại "<<sc_time_stamp()<<endl;
                    // Hoán đổi trạng thái các khay pin
                    if(nameBattery == "Honda"){
                        temp = pinHonda[0];
                        pinHonda[0] = pinHonda[1];
                        pinHonda[1] = temp;
                    }else{
                        temp = pinVinFast[0];
                        pinVinFast[0] = pinVinFast[1];
                        pinVinFast[1] = temp;
                    }
                }
                cout<<"-----------------------------"<<endl;
                cout<<"Kết thúc quá trình tại "<<sc_time_stamp()<<endl;   
                cout<<"-----------------------------"<<endl;
            }
        }
        thread5_done_event.notify();   
    }
};
int sc_main(int argc, char* argv[]) {
  ATMPIN atmPin("atmPin");
  sc_start(3000, SC_SEC);
  return 0;
}