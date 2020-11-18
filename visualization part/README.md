## Data visualization

**Functionality.**

  after the robot collect various data using sensors, then should save each sensor's data into a txt.file in real time and also visualize the measured data on a geographic map. 

<img src="C:\Users\ASUS\Desktop\visualization.jpg" alt="visualization" style="zoom: 33%;" />

​                                                               （expected result）

##### Logic diagram 

<img src="C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20201118122928833.png" alt="image-20201118122928833" style="zoom: 50%;" />



  Saving all the data into a txt file on real time. Configurate Rosbridge that can provides a JSON API to ROS, Edit the html file to activate a Webb server and publish a home page on that server.



**Rosbridge** 

  Rosbridge provides a JSON API to ROS functionality for non-ROS programs. There are a variety of front ends that interface with rosbridge, including a WebSocket server for web browsers to interact with. Rosbridge_suite is a meta-package containing rosbridge, various front end packages for rosbridge like a WebSocket package, and helper packages. (http://wiki.ros.org/rosbridge_suite)

