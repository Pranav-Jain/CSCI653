import polyscope as ps
import numpy as np
import gpytoolbox as gpy
from matplotlib.colors import to_rgb

def sdf_sphere(p):
    return 1. - np.linalg.norm(p,axis=1)

def sdf_link(p):
    le =1.0
    r1 =0.5
    r2 = 0.2
    q_y = np.maximum(np.abs(p[:,1]) - le, 0)
    q = np.stack([p[:,0], q_y, p[:,2]], axis=1)
    lenxy = np.sqrt(q[:,0]**2 + q[:,1]**2)
    return r2 - np.sqrt((lenxy - r1)**2 + q[:,2]**2)

def sdf_capsule(p):
    a = np.array([0,0,0])
    b = np.array([0,2,0])
    r=0.5
    pa = p-a
    ba = b-a
    h = np.clip(np.sum(pa * ba, axis=1) / np.dot(ba, ba), 0.0, 1.0)
    diff = pa - ba * h[:, None]
    return r - np.linalg.norm(diff, axis=1)



if __name__ == "__main__":

    gx, gy, gz = np.meshgrid(np.linspace(-2.0, 2.0, 100), np.linspace(-2.0, 2.0, 100), np.linspace(-2.0, 2.0, 100))
    U = np.vstack((gx.flatten(), gy.flatten(), gz.flatten())).T

    sdf_s = sdf_sphere(U)
    sdf_l = sdf_link(U)
    sdf_c = sdf_capsule(U)

    V_s,F_s = gpy.marching_cubes(sdf_s,U,100,100,100)
    V_l,F_l = gpy.marching_cubes(sdf_l,U,100,100,100)
    V_c,F_c = gpy.marching_cubes(sdf_c,U,100,100,100)

    V_l[:,0] -=3.
    V_c[:,0] -=6.
    V_c[:,1] -=1.
    

    ps.init()
    ps.set_ground_plane_mode("none")
    ps.register_surface_mesh("sphere",V_s,F_s,enabled=True,color=to_rgb("#ABA9A9"))
    ps.register_surface_mesh("link",V_l,F_l,enabled=True,color=to_rgb("#ABA9A9"))
    ps.register_surface_mesh("capsule",V_c,F_c,enabled=True,color=to_rgb("#ABA9A9"))

    ps.show()

