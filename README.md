# CSCI653 Final Project: Parallel Walk on Spheres (WoS)
Repository for course project of CSCI653

## Walk on Spheres

We want to solve a Laplace equation with Dirichlet boundary conditions, namely,
```math

\begin{align*}
\Delta u = 0, \text{on} \space \Omega \\
u = g, \text{on} \space \partial \Omega
\end{align*}
```
the Walk on Sphere (WoS) method is a fast Monte Carlo method that 
* Starts at **a point inside** the problem domain
* Finds the largest sphere centered at that point that fits inside the domain
* Jumps to a **random point on the surface** of that sphere (aka. "Walk on Spheres")
* Repeats until the particle gets close to the boundary
* Uses the boundary condition to estimate the domain
* Randomly repeats the walk N times the take the average of the result

<img width="591" height="261" alt="Image" src="https://github.com/user-attachments/assets/6b41746d-4959-4372-a11d-5273ea82daa1" />

The problem itself is **super** parallelable - Each point inside the domain walks independently, so as its own N walks.

## Parallel WoS

By introducing OpenMp to distribute the Monte-Carlo sampling across multiple CPU cores, we are able to speedup the whole process dramatically. 
Currently we have three simple domain that we tested on: Sphere, Link, and Capsule. 
<img width="1280" height="720" alt="Image" src="https://github.com/user-attachments/assets/29e9ba0f-b15d-4213-a974-74d380b6aa60" />

The runtime comparison as follows:

<img width="1000" height="450" alt="Image" src="https://github.com/user-attachments/assets/57cbe204-a75a-4a26-90d1-6510508041d3" />

<img width="1000" height="450" alt="Image" src="https://github.com/user-attachments/assets/7be687af-6e94-4366-aebe-2554b0b8e8df" />

<img width="1000" height="450" alt="Image" src="https://github.com/user-attachments/assets/1518129b-fcd2-4d1b-9610-281c8bd45351" />


## TODO:
* More interesting geometry domain
* More boundary conditions
* CUDA version
