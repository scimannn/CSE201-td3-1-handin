#include <iostream>
#include "td3.hpp"
#include "support.hpp"
#include <stdlib.h>
#include <math.h>       // sin, cos
#include <assert.h>

using namespace std;

using namespace support;

double* extend_array(double* array, int length, int new_size) {
  // IMPLEMENT YOUR FUNCTION HERE
  double *newArray = new double[new_size];
  for (int i=0; i < new_size; i++)
  {
      if (i < length)
      {
          newArray[i] = array[i];
      }
      else
      {
          newArray[i] = 0;
      }
  }

  delete[] array;

  return newArray; // YOU CAN CHANGE THIS
}

double* shrink_array(double* array, int length, int new_size) {
  // IMPLEMENT YOUR FUNCTION HERE
  double *newArray = new double[new_size];
  for (int i=0; i < new_size; i++)
  {
      newArray[i] = array[i];
  }
  delete[] array;
  return newArray; // YOU CAN CHANGE THIS
}

double* append_to_array(double element,
                        double* array,
                        int &current_size,
                        int &max_size) {
  // IMPLEMENT YOUR FUNCTION HERE
  //std::cout << element << " " << current_size << " " << max_size << std::endl;
  if (current_size == max_size)
  {
      array = extend_array(array, current_size, current_size+5);
      max_size += 5;
  }
  current_size += 1;
  array[current_size-1] = element ;


  return array; // YOU CAN CHANGE THIS
}

double* append_to_array_copy(double element,
                        double* array,
                        int &current_size,
                        int &max_size) {
  // IMPLEMENT YOUR FUNCTION HERE
  std::cout << element << " " << current_size << " " << max_size << std::endl;
  if (current_size == max_size)
  {
    array = extend_array(array, current_size, current_size+5);
    max_size += 5;
  }
  current_size += 1;
  array[current_size-1] = element ;

    //std::cout << "Displaying array" << std::endl;
    //for (int i=0; i < current_size; i++)
    //{
    //  std::cout << array[i] << std::endl;
    //}


  return array; // YOU CAN CHANGE THIS
}

double* remove_from_array(double* array,
                          int &current_size,
                          int &max_size) {
  // IMPLEMENT YOUR FUNCTION HERE
  current_size -= 1;
  if (max_size - current_size >= 5)
  {
      array = shrink_array(array, max_size, current_size);
      max_size -= (max_size - current_size);
  }

  return array; // YOU CAN CHANGE THIS
}

bool simulate_projectile(const double magnitude, const double angle,
                         const double simulation_interval,
                         double *targets, int &tot_targets,
                         int *obstacles, int tot_obstacles,
                         double* &telemetry,
                         int &telemetry_current_size,
                         int &telemetry_max_size) {
  // YOU CAN MODIFY THIS FUNCTION TO RECORD THE TELEMETRY

  bool hit_target, hit_obstacle;
  double v0_x, v0_y, x, y, t;
  double PI = 3.14159265;
  double g = 9.8;

  v0_x = magnitude * cos(angle * PI / 180);
  v0_y = magnitude * sin(angle * PI / 180);

  t = 0;
  x = 0;
  y = 0;

  hit_target = false;
  hit_obstacle = false;
  while (y >= 0 && (! hit_target) && (! hit_obstacle)) {
    double * target_coordinates = find_collision(x, y, targets, tot_targets);
      telemetry = append_to_array(x, telemetry, telemetry_current_size, telemetry_max_size);
      telemetry = append_to_array(y, telemetry, telemetry_current_size, telemetry_max_size);
      telemetry = append_to_array(t, telemetry, telemetry_current_size, telemetry_max_size);
    if (target_coordinates != NULL) {
      remove_target(targets, tot_targets, target_coordinates);
      hit_target = true;
    } else if (find_collision(x, y, obstacles, tot_obstacles) != NULL) {
      hit_obstacle = true;
    } else {
      t = t + simulation_interval;
      y = v0_y * t  - 0.5 * g * t * t;
      x = v0_x * t;
    }
  }

  return hit_target;
}

void sort(double *global_telemetry, const int tot_elements) {
  // IMPLEMENT YOUR CODE HERE
  bool swapped = true;
  int meaningful_length = tot_elements;
  while (swapped)
  {
    swapped = false;
    for (int i = 3;  i < meaningful_length; i +=3)
    {
      if (global_telemetry[i-3] > global_telemetry[i])
      {
          double t = global_telemetry[i-3];
          double x = global_telemetry[i-2];
          double y = global_telemetry[i-1];
          global_telemetry[i-3] = global_telemetry[i];
          global_telemetry[i-2] = global_telemetry[i+1];
          global_telemetry[i-1] = global_telemetry[i+2];
          global_telemetry[i] = t;
          global_telemetry[i+1] = x;
          global_telemetry[i+2] = y;
          swapped = true;
      }
      meaningful_length -= 3;
    }

  }
}

void merge_telemetry(double **telemetries,
                     int tot_telemetries,
                     int *telemetries_sizes,
                     double* &global_telemetry,
                     int &global_telemetry_current_size,
                     int &global_telemetry_max_size) {
  // IMPLEMENT YOUR FUNCTION HERE
  for (int i = 0; i < tot_telemetries; i++)
  {
    //std::cout << telemetries_sizes[i] << "Total" << std::endl;
    for (int j = 0; j < telemetries_sizes[i]; j++)
    {
     //std::cout << j << std::endl;
      //std::cout << *global_telemetry << std::endl;
      std::cout << telemetries[i][j] << std::endl;
      global_telemetry = append_to_array(telemetries[i][j], global_telemetry, global_telemetry_current_size, global_telemetry_max_size);

    }
  }

  sort(global_telemetry, global_telemetry_current_size);

}


