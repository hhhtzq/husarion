roslaunch my_object_recognition_pkg start_find_object_3d_session.launch
roslaunch myrobot_moveit_config myrobot_planning_execution.launch
rviz
rosrun my_grasping grasp_demo.py
rosenv
source ~/catkin_ws/devel/setup.bash 
rosenv
