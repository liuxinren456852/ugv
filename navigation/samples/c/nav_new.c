/*!
  \author Satofumi KAMIMURA

  $Id: get_distance.c,v 2a7545b3f397 2014/08/27 01:26:02 jun $
*/



#include "urg_sensor.h"
#include "urg_utils.h"
#include "open_urg_sensor.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>




static void print_data_test(urg_t *urg, long data[], int data_n, long time_stamp)
{




  int step;
  int frontIndex;
  (void)data_n;
  int leftState;
  int rightState;
  int frontState;

  for (step = -341; step <340 ; step++) {
    frontIndex = urg_step2index(urg, step);
    printf("frontIndex : %ld \n", frontIndex);
    if (frontIndex < 235) {
      printf(" position : left ");
      if (data[step] < 50) {
        printf(" test ");

      }
    }

    else if (frontIndex >= 235 & frontIndex < 445) {
      printf(" position : front ");
      if (data[step] < 50) {
        // digitalWrite(0, HIGH);
        // digitalWrite(1, LOW);
        // digitalWrite(2, LOW);

      }
    }

    if (frontIndex >= 445 & frontIndex < 682) {
      printf(" position : right ");
      if (data[step] < 50) {
        // digitalWrite(0, LOW);
        // digitalWrite(1, LOW);
        // digitalWrite(2, HIGH);

      }
    }


    // printf("%ld [mm], (%ld [msec])\n", data[front], time_stamp);
  }


}





static void print_data(urg_t *urg, long data[], int data_n, long time_stamp)
{
#if 1
    int front_index;

    (void)data_n;


    front_index = urg_step2index(urg, 0);
    printf("front : %ld \n", front_index);
    printf("%ld [mm], (%ld [msec])\n", data[front_index], time_stamp);

#else
    (void)time_stamp;

    int i;
    long min_distance;
    long max_distance;


    urg_distance_min_max(urg, &min_distance, &max_distance);
    for (i = 0; i < data_n; ++i) {
        long l = data[i];
        double radian;
        long x;
        long y;

        if ((l <= min_distance) || (l >= max_distance)) {
            continue;
        }
        radian = urg_index2rad(urg, i);
        x = (long)(l * cos(radian));
        y = (long)(l * sin(radian));
        printf("(%ld, %ld), ", x, y);
    }
    printf("\n");
#endif
}


int main(int argc, char *argv[])
{

  wiringPiSetup();

  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);



    enum {
        CAPTURE_TIMES = 2,
    };
    urg_t urg;
    long *data = NULL;
    long time_stamp;
    int n;
    int i;

    if (open_urg_sensor(&urg, argc, argv) < 0) {
        return 1;
    }

    data = (long *)malloc(urg_max_data_size(&urg) * sizeof(data[0]));
    if (!data) {
        perror("urg_max_index()");
        return 1;
    }


#if 0

    urg_set_scanning_parameter(&urg,
                               urg_deg2step(&urg, -90),
                               urg_deg2step(&urg, +90), 0);

#endif

    urg_start_measurement(&urg, URG_DISTANCE, URG_SCAN_INFINITY, 0);
     for (i = 0; i < CAPTURE_TIMES; ++i) {

        n = urg_get_distance(&urg, data, &time_stamp);
        if (n <= 0) {
            printf("urg_get_distance: %s\n", urg_error(&urg));
            free(data);
            urg_close(&urg);
            return 1;
        }
        delay(100);
        print_data_test(&urg, data, n, time_stamp);
     }


    free(data);
    urg_close(&urg);

#if defined(URG_MSC)
    getchar();
#endif
    return 0;
}
