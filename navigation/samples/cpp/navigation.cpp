
#include "Urg_driver.h"
#include "Connection_information.h"
#include "math_utilities.h"
#include <iostream>
#include <wiringPi.h>


using namespace qrk;
using namespace std;


namespace
{

  int step;
  int frontIndex;
  int leftState;
  int rightState;
  int frontState;

    void print_data(const Urg_driver& urg,
                    const vector<long>& data, long time_stamp)
    {
#if 1

        int front_index = urg.step2index(0);
        cout << data[front_index] << " [mm], ("
             << time_stamp << " [msec])" << endl;

#else

        long min_distance = urg.min_distance();
        long max_distance = urg.max_distance();
        size_t data_n = data.size();
        for (size_t i = 0; i < data_n; ++i) {
            long l = data[i];
            if ((l <= min_distance) || (l >= max_distance)) {
                continue;
            }

            double radian = urg.index2rad(i);
            long x = static_cast<long>(l * cos(radian));
            long y = static_cast<long>(l * sin(radian));
            cout << "(" << x << ", " << y << ")" << endl;
        }
        cout << endl;
#endif
    }

    void navigate(const Urg_driver& urg,
                    const vector<long>& data, long time_stamp)
    {
      wiringPiSetup();

      pinMode(0, OUTPUT);
      pinMode(1, OUTPUT);
      pinMode(2, OUTPUT);


      for (frontIndex = 1; frontIndex < 682 ; frontIndex++) {

        //cout << "frontIndex : " << frontIndex << endl;
        if (data[frontIndex] < 500 && data[frontIndex] > 50 ) {
          if (frontIndex < 235) {


              digitalWrite(0, HIGH);
              digitalWrite(1, LOW);
              digitalWrite(2, LOW);
              cout <<"left : " << data[frontIndex] << endl;
              //serialPutchar(fd, 'l');


          }


           if (frontIndex >= 235 && frontIndex < 445) {
            //cout <<" position : front ";

              digitalWrite(0, HIGH);
              digitalWrite(1, LOW);
              digitalWrite(2, LOW);
              //serialPutchar(fd, 'f');
              cout <<"front : " << data[frontIndex]<< endl;

          }



            if (frontIndex >= 445 && frontIndex < 682) {
              digitalWrite(0, LOW);
              digitalWrite(1, LOW);
              digitalWrite(2, HIGH);
              cout <<"right : " << data[frontIndex] << endl;
              //serialPutchar(fd, 'r');

          }

          // else { cout << "out of range" << endl;}
        }

        else { cout << "no obstacle";

}



    }
}



}


int main(int argc, char *argv[])
{

  wiringPiSetup();



  // int fd ;
  // if ((fd = serialOpen ("/dev/ttyACM1", 9600)) < 0)
  // {
  //   fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
  //   // return 1 ;
  // }
  //
  // if (wiringPiSetup () == -1)
  // {
  //   fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
  //   // return 1 ;
  // }

  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);


    Connection_information information(argc, argv);


    Urg_driver urg;
    if (!urg.open(information.device_or_ip_name(),
                  information.baudrate_or_port_number(),
                  information.connection_type())) {
        cout << "Urg_driver::open(): "
             << information.device_or_ip_name() << ": " << urg.what() << endl;
        return 1;
    }


#if 1

    urg.set_scanning_parameter(urg.deg2step(-90), urg.deg2step(+90), 0);
#endif
    enum { Capture_times = 10 };
    urg.start_measurement(Urg_driver::Distance, Urg_driver::Infinity_times, 0);
    //for (int i = 0; i < Capture_times; ++i) {
while(1){
        vector<long> data;
        long time_stamp = 0;

        if (!urg.get_distance(data, &time_stamp)) {
            cout << "Urg_driver::get_distance(): " << urg.what() << endl;
            return 1;
        }
        navigate(urg, data, time_stamp);
        cout << endl;
delay(10);


    }

#if defined(URG_MSC)
    getchar();
#endif
    return 0;
}
