#include <iostream>
#include <stdio.h>
#include <math.h>
#include <vacuumms/types.h>
#include <vacuumms/constants.h>
#include <vacuumms/variational/variational.hh>
#include <vacuumms/variational/quaternion.hh>

// These are provided and initialized in constants.h
//  extern vacuumms_float epsilon;
//  extern vacuumms_float sqrt_epsilon;

void Variational2D::init(vacuumms_float _start_x, 
                    vacuumms_float _start_y, 
                    vacuumms_float _end_x, 
                    vacuumms_float _end_y, 
                    vacuumms_float _sigma, 
                    vacuumms_float _epsilon, 
                    int _n_var_points)
{
    // grab the passed values

    start_x = _start_x; 
    start_y = _start_y; 
    end_x = _end_x; 
    end_y = _end_y; 
    sigma = _sigma;
    epsilon = _epsilon;
    n_var_points = _n_var_points;


    // start constructing 

    var_x = new vacuumms_float[n_var_points];
    var_y = new vacuumms_float[n_var_points];


    // initialize set of points
    
    for (int i=0; i<n_var_points; i++)
    {
        var_x[i] = start_x + (i + 1) * (end_x - start_x) / (n_var_points + 1);
        var_y[i] = start_y + (i + 1) * (end_y - start_y) / (n_var_points + 1);
    }
}


/* This constructor uses a pointer to an explicit energy function,
 * a mostly deprecated functionality, as this has been implemented
 * as part of the Configuration class. If invoked, the configuration
 * class member will be a nullptr, indicating this energy function
 * should be used.
 */

Variational2D::Variational2D(vacuumms_float _start_x, 
                             vacuumms_float _start_y, 
                             vacuumms_float _end_x, 
                             vacuumms_float _end_y, 
                             vacuumms_float _sigma, 
                             vacuumms_float _epsilon, 
                             int _n_var_points, 
                             vacuumms_float(*_energy_function)(vacuumms_float x, vacuumms_float y))
{
    init(_start_x, _start_y, _end_x, _end_y, sigma, epsilon, _n_var_points);
    use_configuration_energy = false;
    configuration = nullptr;
    energy_function = _energy_function;
}


Variational2D::Variational2D(vacuumms_float _start_x, 
                             vacuumms_float _start_y, 
                             vacuumms_float _end_x, 
                             vacuumms_float _end_y, 
                             vacuumms_float _sigma, 
                             vacuumms_float _epsilon, 
                             int _n_var_points, 
                             Configuration *_configuration)
{
    init(_start_x, _start_y, _end_x, _end_y, _sigma, _epsilon, _n_var_points);
    use_configuration_energy = true;
    configuration = _configuration;
    energy_function = nullptr;
}


Variational2D::~Variational2D()
{
    delete var_x, var_y;
}


void Variational2D::printValues()
{
    std::cout << "#####" << start_x << " " << start_y << std::endl;
    for (int i=0; i<n_var_points; i++)
        std::cout << "#####" << var_x[i] << " " << var_y[i] << std::endl;
    std::cout << "#####" << end_x << " " << end_y << std::endl;
}
    

void Variational2D::iterate()
{
    vacuumms_float new_x[n_var_points], new_y[n_var_points];
    // looking along direction of line, point before and point after
    vacuumms_float fore_x, aft_x, fore_y, aft_y;

    for (int i=0; i<n_var_points; i++)
    {
        if (i>0) fore_x = var_x[i-1];
        else fore_x = start_x;
        if (i+1<n_var_points) aft_x = var_x[i+1];
        else aft_x = end_x;
        if (i>0) fore_y = var_y[i-1];
        else fore_y = start_y;
        if (i+1<n_var_points) aft_y = var_y[i+1];
        else aft_y = end_y;

        // sample directional derivative (direction perpendicular to curve)

        // get direction perpendicular, negative reciprocal of slope
        vacuumms_float directional_y = -(aft_x - fore_x);
        vacuumms_float directional_x = (aft_y - fore_y);

        // normalize the direction vector
        vacuumms_float directional_magnitude = sqrt(directional_x*directional_x + directional_y*directional_y);
        directional_x /= directional_magnitude;
        directional_y /= directional_magnitude;

        if (getenv("VACUUMMS_DEBUG") != NULL)
        {
            std::cout << "direction for var point " << i 
                      << ": (" << var_x[i] << ", " << var_y[i] 
                      << "):(" << directional_x << ", " 
                      << directional_y << ")" << "\tmagnitude: " 
                      << sqrt(directional_x * directional_x + directional_y * directional_y) 
                      << std::endl;
        }

        // resize the direction vector to machine epsilon
        directional_x *= sqrt_machine_epsilon;
        directional_y *= sqrt_machine_epsilon;

        char *debug = getenv("VACUUMMS_DEBUG");
        if (debug != NULL)
        {
            printf("using resized directional x,y: %f, %f\n", directional_x, directional_y);
            printf("sqrt_epsilon = %0.012f\n", sqrt_machine_epsilon);
        }

        // sample energy to evaluate derivative
        vacuumms_float sample_left_x = var_x[i] - directional_x;
        vacuumms_float sample_left_y = var_y[i] - directional_y;
        vacuumms_float sample_right_x = var_x[i] + directional_x;
        vacuumms_float sample_right_y = var_y[i] + directional_y;

        vacuumms_float energy_left;
        vacuumms_float energy_right;

        if (use_configuration_energy)
        {
            energy_left = configuration->insertionEnergy(sample_left_x, sample_left_y, 0, sigma, epsilon);
            energy_right = configuration->insertionEnergy(sample_right_x, sample_right_y, 0, sigma, epsilon);
        }
        else 
        {
            energy_left = energy_function(sample_left_x, sample_left_y);
            energy_right = energy_function(sample_right_x, sample_right_y);
        }

        {
            char *debug = getenv("VACUUMMS_DEBUG");
            if (debug != NULL) printf("energy left/right: %.012f <--> %.012f\n", energy_left, energy_right);
        }

        vacuumms_float dE = energy_right - energy_left;

        // update position
        new_x[i] = var_x[i] - alpha * dE * directional_x;
        new_y[i] = var_y[i] - alpha * dE * directional_y;
    }

    // FTW: After full pass, copy the updates back.
    for (int i=0; i<n_var_points; i++)
    {
        var_x[i] = new_x[i];
        var_y[i] = new_y[i];
    }

}


vacuumms_float Variational2D::rebalancePoints2D()
{
    // get the total length of curve
    vacuumms_float prev_x = start_x, prev_y = start_y;
    vacuumms_float curve_length = 0;

    // add distance to each variational point
    for (int i=0; i < n_var_points; i++)
    {
        curve_length += sqrt((var_x[i] - prev_x) * (var_x[i] - prev_x) + (var_y[i] - prev_y) * (var_y[i] - prev_y));

        // update for next iteration
        prev_x = var_x[i]; 
        prev_y = var_y[i];
    }

    // add the last piece
    curve_length += sqrt((end_x - prev_x) * (end_x - prev_x) + (end_y - prev_y) * (end_y - prev_y));
    // Cut each new segment along old path to this length
    vacuumms_float new_segment_length = curve_length / (n_var_points + 1);


    // now the respace
    vacuumms_float cursor_x = start_x;
    vacuumms_float cursor_y = start_y;
    vacuumms_float remainder_x = 0;         
    vacuumms_float remainder_y = 0;         
    vacuumms_float remainder = 0;
    int old_point = 0;

    vacuumms_float new_var_x[n_var_points];
    vacuumms_float new_var_y[n_var_points];

    for (int i=0; i<n_var_points; ) 
    {
        // grab old segments until there's enough to cut at least one new one
        // while (remainder_x < new_segment_length)
        while (remainder < new_segment_length)
        {
            vacuumms_float segment_x, segment_y;
            if (old_point < n_var_points) 
            {
                segment_x = var_x[old_point] - cursor_x;
                segment_y = var_y[old_point] - cursor_y;
            }
            else 
            {
                segment_x = end_x - cursor_x;
                segment_y = end_y - cursor_y;
            }
            remainder_x += segment_x;
            remainder_y += segment_y;
            remainder = sqrt(remainder_x*remainder_x + remainder_y*remainder_y);
            cursor_x = var_x[old_point];
            cursor_y = var_y[old_point];
            old_point++;
        }
        
        // now cut new segments until there's not enough left
        // need to cover case equality of segment lengths, so adding it here
        while (new_segment_length <= remainder)
        {
            // get projections of new_segment_length onto x, y directions
            vacuumms_float delta_x = (remainder_x / remainder) * new_segment_length;
            vacuumms_float delta_y = (remainder_y / remainder) * new_segment_length;
            if (i==0) 
            {
                new_var_x[i] = start_x + delta_x;
                new_var_y[i] = start_y + delta_y;
            }
            else
            {
                new_var_x[i] = new_var_x[i-1] + delta_x;
                new_var_y[i] = new_var_y[i-1] + delta_y;
            }
            remainder_x -= delta_x;
            remainder_y -= delta_y;
            remainder = sqrt(remainder_x*remainder_x + remainder_y*remainder_y);
            i++; // move to next new point
        }            
    }

    // check new curve length, add distance to each variational point
    vacuumms_float new_curve_length = 0;
    prev_x = start_x, prev_y = start_y;
    
    for (int i=0; i < n_var_points; i++)
    {
        new_curve_length += sqrt((new_var_x[i] - prev_x) * (new_var_x[i] - prev_x) + (new_var_y[i] - prev_y) * (new_var_y[i] - prev_y));

        // update for next iteration
        prev_x = new_var_x[i]; 
        prev_y = new_var_y[i];
    }

    // add the last piece
    new_curve_length += sqrt((end_x - prev_x) * (end_x - prev_x) + (end_y - prev_y) * (end_y - prev_y));
    
    vacuumms_float shrinkage = new_curve_length / curve_length;

    // copy out
    for (int i=0; i<n_var_points; i++)
    {
        var_x[i] = new_var_x[i];
        var_y[i] = new_var_y[i];
    }
   
    return shrinkage;
}
