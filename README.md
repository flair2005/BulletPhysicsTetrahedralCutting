# Bullet Physics Tetrahedral Voluemtric Cutting

This is a extension to the Bullet Physics Library that allows for fast efficient cutting of Tetrahedralized meshes at near real-time speeds. We use btSoftbody to handle the main physics simulation of our tetrahedral mesh in the btSoftbody world we implemented a function in the internal step simulation to check if a tetrahedron is colliding with rigidbody that we label as a rigidbody that cuts softbody by a bool value added to the btRigidBody.h class. When we collide with these supposed cutter rigidbodies we then remove linkes based of the tetrahdral mesh edges that are pre-calculated when the tetrahedral mesh is loaded. There are currently to algorithms to cut tetrahedral meshes implemented. We have 1 have simply break linkes based on whether or not it collides. The second algorithm uses the accumulated force length in each tetrahedron node to determine the amount of stress placed on a tetrahedron and we have a value defined in the btSoftbody to determine the stress break threshold and if the acculated force exceeds this then we will break the links. This method allows for a fast, efficient cutting a tetrahedral meshes instead of cutting through the tetrahedral mesh via raycast because were avoiding have to recompute the tetrahedralization of the mesh.

Devolped by: Alpha Shabazz
For: RoketWorks
Contact: alphashabazz1@gmail.com
Code: C++

Copyright (C) RoketWorks, Inc - All Rights Reserved
Unauthorized copying of this file, via any medium is strictly prohibited
Proprietary and confidential
Written by Alpha Shabazz <alphashabazz1@gmail.com>, January 2015

