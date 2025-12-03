import csv
import polyscope as ps
import numpy as np

if __name__ == "__main__":
    ps.init()
    
    # Load points and values from CSV
    points = []
    values = []
    with open("../results/sphere.csv", "r") as f:
        reader = csv.reader(f)
        for row in reader:
            x, y, z, val = map(float, row)
            points.append([x, y, z])
            values.append(val)
    
    points = np.array(points)
    values = np.array(values)
    
    # Register point cloud with Polyscope
    ps.register_point_cloud("Points", points)
    ps.get_point_cloud("Points").add_scalar_quantity("Values", values)
    
    ps.show()
