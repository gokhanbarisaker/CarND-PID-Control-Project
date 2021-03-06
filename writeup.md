# PID 


**PID Project**

The goals / steps of this project are the following:

* Follow Q&A section to familiarise with how system works.
* Implement PID internals.
* Make sure steering angle is generated by PID controller.
* Explore best practices to define PID constants
* Apply those best practices
* Summarize the results with a written report


[//]: # (Asset References)

// TODO: Stack sample photos to support ideas

[video1]: ./examples/pid-ok.mov "Target distance exceeded"
[image2]: ./examples/speed.png "Car drives under the highway speed limit"
[image3]: ./examples/collision.png "Car driving without having any collision with other cars"

## Rubric Points
### Here I will consider the [rubric points](https://review.udacity.com/#!/rubrics/1972/view) individually and describe how I addressed each point in my implementation.  

---
### Compilation

#### 1. Your code should compile.

My project modifies the following files:
* main.cpp, binds PID controller into event processing loop
* PID.(c/h)pp, adds PID controller implementation to the project

The project also includes some docker images and shell scripts to have a easy setup without changing anything in the running machine 
* Base.dockerfile, a docker image including all the project dependencies
* base.sh, a shell file to build base docker image
* Project.dockerfile, a docker image built on top of `Base.dockerfile`. It builds & run the project source code.
* run.sh, a shell file to build and run the `Project.dockerfile`
* kill.sh, a shell file for killing currently running docker image. (Has a lot of room for improvement :))




### Implementation

#### 1. The PID procedure follows what was taught in the lessons.

Project introduces a PID implementation that aims to strictly follow the knowledge provided in the lecture.

It uses `Kp * p_error + Ki * i_error + Kd * d_error` formula to calculate the total error, where ...

* Kp, the constant for proportional part of the error
* p_error, the error for the last known state
* Ki, the constant for integral part of the error
* i_error, the sum of all prior error observations
* Kd, the constant for differential part of the error
* d_error, the change in error within last and current observation proportional to the elapsed time.


### Reflection
####Describe the effect each of the P, I, D components had in your implementation.
Project with initial boilerplate code allows car to drive straight in cartesian coordinates. This eventually results the car to drift away from the road as the lane starts to rotate.

[Click here for the video](examples/pid-0k.mov)
Please checkout the path/to/repo/examples/pid-ok.mov if the link doesn't work 

To observe the impact of PID controller, I initialized a new PID instance and start using it to generate steering values.

```
PID pid_steer;
pid_steer.Init(0.0, 0.0, 0.0);

def onMessage():
  pid_steer.UpdateError(cte);
  steering_angle = pid_steer.TotalError();
```

First iteration had all constants set to 0. Eventually, that didn't create an impact (as expected from the total error implementation) and caused the car to drift away by keeping it at the same route as before.

With the second iteration, I decided to play around with the Kp (proportional error constant). 

The third iteration aimed to adjust the Kp (proportional error constant) value. I observed that the higher it diverges from 0 towards negative direction, the eager vehicle tries to steer towards the aimed position and vice versa.

**Impact of positive Kp value on steering**

[Click here for the video](examples/p-pos.mov)
Please checkout the path/to/repo/examples/pid-pos.mov if the link doesn't work

**Impact of negative Kp value on steering**

[Click here for the video](examples/p-neg.mov)
Please checkout the path/to/repo/examples/p-neg.mov if the link doesn't work

As a result, negative Kp value proved to be useful to keep us on the lane until its fluctuation  raises to a level that swings us out of the lane. Unfortunately, just scaling down the Kp value reduces the eagerness to approach to the aimed position that leads to similar results with our previous try where all constants initialized with 0 value.

The forth iteration aimed to adjust the Kd (differential error constant) value. I observed that the higher it diverges from 0 towards negative direction, the vehicle trajectory overshoots less and vice versa.

**Impact of positive Kd value on steering**

[Click here for the video](examples/pd-pos.mov)
Please checkout the path/to/repo/examples/pd-pos.mov if the link doesn't work

**Impact of negative Kd value on steering**

[Click here for the video](examples/pd-neg.mov)
Please checkout the path/to/repo/examples/pd-neg.mov if the link doesn't work

As a result, negative Kd value seemed to balance out eagerness of proportional part and helped us to keep the vehicle in the lane for the longer run.

I skipped playing around with the integral part since there doesn't seem to be a systematic bias in the simulation. However, that would have helped us for preventing constant shift from the desired position.

Thus, for this project this project uses Ki with value 0.

One estimated potential usage could be with speed/throttle values, where we apply bias towards positive value.


#### Describe how the final hyperparameters were chosen.
For optimizing choosen hyperparameters, I considered manual calibration and twiddle methods.

Twiddle seemed to be impossible to implement since there was no comparable way to reset car position to the starting point to simulate the total error with the same conditions.

That led me to use manual calibration method. This one took a while, however, I settled in with [-0.08, 0.0, -100.0] PID values. It turns out a small P value is enough to set an eager path to converge towards goal. However, to balance that beast we need a higher differential value.

### Simulation
#### The vehicle must successfully drive a lap around the track.
Cruises relatively smooth without rolling over in the track.



