from __future__ import print_function
from __future__ import division
from __future__ import absolute_import

import numpy as np

def getRotateMat(theta1, theta2, theta3):
    theta1 = theta1 * np.pi / 180
    theta2 = theta2 * np.pi / 180
    theta3 = theta3 * np.pi / 180
    rotate_mat_x = np.array([
        [1,              0,               0],
        [0, np.cos(theta1), -np.sin(theta1)],
        [0, np.sin(theta1),  np.cos(theta1)],
    ])
    

    rotate_mat_y = np.array([
        [np.cos(theta2), 0, -np.sin(theta2)],
        [0,              1,               0],
        [np.sin(theta2), 0,  np.cos(theta2)],
    ])

    rotate_mat_z = np.array([
        [np.cos(theta3), -np.sin(theta3), 0],
        [np.sin(theta3),  np.cos(theta3), 0],
        [0,              0,               1],
    ])

    rotate_mat = np.matmul(np.matmul(rotate_mat_x, rotate_mat_y), rotate_mat_z)

    return rotate_mat

def generateCylinderData(radius=0.5, height=1.6, \
                         N_radius_split=50, N_angle_split=100, N_height_split=100):
    # radius: the radius of the circle face
    # height: the height of the cylinder
    # N_radius_split: the number of intervals of the divied radius
    # N_angle_split: the number of intervals of the divied angle
    # N_height_split: the number of intervals of the divied height
    # x^2 + y^2 = radius^2 --> x = radius * cos(theta), y = radius * sin(theta)
    # assert (N_circle % 3 == 0 and N_side % 3 == 0)
    front_circle = []
    behind_circle = []
    side_face = []

    front_circle_inds = []
    # behind_circle_inds = []
    side_face_inds = []

    delta_theta = 2 * np.pi / N_angle_split
    delta_r = radius / N_radius_split
    delta_h = height / N_height_split

    for i in range(N_radius_split + 1):
        for j in range(N_angle_split + 1):
            _theta = j * delta_theta
            _radius = i * delta_r
            front_circle.append([_radius * np.cos(_theta),
                                 _radius * np.sin(_theta),
                                 height / 2])
            behind_circle.append([_radius * np.cos(_theta),
                                 _radius * np.sin(_theta),
                                 - height / 2])
            if i < N_radius_split and j < N_angle_split:
                front_circle_inds.append([i*(N_angle_split+1)+j, (i+1)*(N_angle_split+1)+j, (i+1)*(N_angle_split+1)+j+1])
                front_circle_inds.append([i*(N_angle_split+1)+j, (i+1)*(N_angle_split+1)+j+1, i*(N_angle_split+1)+j+1])
            

    for i in range(N_height_split + 1):
        for j in range(N_angle_split + 1):
            _theta = j * delta_theta
            side_face.append([
                radius * np.cos(_theta),
                radius * np.sin(_theta),
                - height / 2 + i * delta_h,
            ])

            if i < N_height_split and j < N_angle_split:
                side_face_inds.append([i*(N_angle_split+1)+j, (i+1)*(N_angle_split+1)+j, (i+1)*(N_angle_split+1)+j+1])
                side_face_inds.append([i*(N_angle_split+1)+j, (i+1)*(N_angle_split+1)+j+1, i*(N_angle_split+1)+j+1])

    front_circle_np = np.vstack(front_circle)
    behind_circle_np = np.vstack(behind_circle)
    side_face_np = np.vstack(side_face)

    front_circle_inds_up = np.vstack(front_circle_inds).ravel()
    behind_circle_inds_np = front_circle_inds_up + front_circle_np.shape[0]
    side_face_inds_np = np.stack(side_face_inds).ravel() + front_circle_np.shape[0] * 2

    end_points = {
        'front_circle_np': front_circle_np,
        'behind_circle_np': behind_circle_np,
        'side_face_np': side_face_np,
        'front_circle_inds_up': front_circle_inds_up,
        'behind_circle_inds_np': behind_circle_inds_np,
        'side_face_inds_np': side_face_inds_np,
    }

    print(front_circle_np.shape, behind_circle_np.shape, side_face_np.shape, \
        front_circle_inds_up.shape, behind_circle_inds_np.shape, side_face_inds_np.shape)

    return end_points

def generateBallData(radius=1.0, intervals=50):
    delta_angle = 2.0 * np.pi / intervals
    vertices = []
    inds = []

    for i in range(intervals + 1):
        for j in range(intervals + 1):
            theta = i * 1.0 * delta_angle
            alpha = j * 1.0 * delta_angle

            x = radius * np.cos(alpha) * np.cos(theta)
            y = radius * np.cos(alpha) * np.sin(theta)
            z = radius * np.sin(alpha)

            # normal vector
            nx = x / radius
            ny = y / radius
            nz = z / radius

            vertices.append([x, y, z, nx, ny, nz])

            if i != intervals and j != intervals:
                inds.append([
                    i * (intervals + 1) + j,
                    (i + 1) * (intervals + 1) + j,
                    (i + 1) * (intervals + 1) + j + 1,
                    i * (intervals + 1) + j,
                    (i + 1) * (intervals + 1) + j + 1,
                    i * (intervals + 1) + j + 1,
                ])
    vertices = np.vstack(vertices)
    inds = np.vstack(inds)

    print(vertices.shape, inds.shape)

    np.savetxt(fname="ball_inds.txt", X=inds.reshape(-1), delimiter=',', fmt='%8d')
    np.savetxt(fname="ball_vertices.txt", X=vertices, delimiter=',', fmt='%.6f')


def rotate_raw_data(theta1, theta2, theta3):
    rotate_mat = getRotateMat(theta1, theta2, theta3)
    end_points = generateCylinderData()
    end_points_new = {
        'front_circle_np': np.matmul(end_points['front_circle_np'], rotate_mat.T),
        'behind_circle_np': np.matmul(end_points['behind_circle_np'], rotate_mat.T),
        'side_face_np': np.matmul(end_points['side_face_np'], rotate_mat.T),
        'front_circle_inds_up': end_points['front_circle_inds_up'],
        'behind_circle_inds_np': end_points['behind_circle_inds_np'],
        'side_face_inds_np': end_points['side_face_inds_np'],
    }
    return end_points_new


if __name__ == '__main__':

    # theta = [45, 45, 45]
    # rotate_mat = getRotateMat(*theta)
    # end_points = rotate_raw_data(*theta)

    # # print(end_points['front_circle_np'].shape, end_points['behind_circle_np'].shape, end_points['side_face_np'].shape)
    # X = np.concatenate((end_points['front_circle_np'], end_points['behind_circle_np'], end_points['side_face_np']), axis=0)
    # print(X.shape)
    # np.savetxt(fname="vertices.txt", X=X, delimiter=',', fmt='%.6f')
    # # print(end_points['front_circle_inds_up'].shape, end_points['behind_circle_inds_np'].shape, end_points['side_face_inds_np'].shape)
    # INDS = np.concatenate((end_points['front_circle_inds_up'], end_points['behind_circle_inds_np'], end_points['side_face_inds_np']), axis=0).astype(np.int32)
    # print(INDS.shape)
    # np.savetxt(fname="inds.txt", X=INDS, delimiter=',', fmt='%8d')

    generateBallData(radius=1.0)
