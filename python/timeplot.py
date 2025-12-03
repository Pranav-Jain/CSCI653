import numpy as np
import matplotlib.pyplot as plt
import os

outputpath = "results"
os.makedirs(outputpath,exist_ok=True)

# data 1: same walk, different sample points
parallel_1 = [131.303,217.865,418.2,
            339.743,307.597,386.36,453.923,
            475.057,539.972]

serial_1 = [388.94,771.872,1153.82,1540.42,
          1927.34,2280.29,2749.91,3103.42,3432.23]

sample_pts = [200,400,600,800,1000,1200,1400,1600,1800]
walk = [500,1000,1500,2000,2500,3000,3500,4000,4500]

fig = plt.figure(figsize=(15,6))
ax1 = fig.add_subplot(121)

ax1.plot(sample_pts,serial_1,label='serial',marker='o',lw=2.,markersize=4)
ax1.plot(sample_pts,parallel_1,label='parallel',marker='P',lw=2.,markersize=4)
ax1.plot(sample_pts,sample_pts,label='O(M)',linestyle='--',color='k',lw=2)

ax1.set_xscale('log')
ax1.set_yscale('log')
ax1.legend()
ax1.set_xlabel("log(sample points M)")
ax1.set_ylabel("log(runtime (ms))")
ax1.set_title("Parallel v.s. Serial WoS runtime with 2000 walks")

# data2: same sample points, different walks

parallel_2 = [143.05,244.327,375.332,435.796,438.878,
            666.707,728.364,667.396,781.271]
serial_2 = [502.359,1011.35,1484.98,1950.27,2468.3,2877.74,
          3418.69,3999.3,4565.76]

ax2 = fig.add_subplot(122)
ax2.plot(walk,serial_2,label='serial',marker='o',lw=2.,markersize=4.)
ax2.plot(walk,parallel_2,label='parallel',marker='P',lw=2.,markersize=4.)
ax2.plot(walk,walk,color='k',lw=2.,linestyle='--',label='O(N)')
ax2.set_xscale('log')
ax2.set_yscale('log')
ax2.legend()
ax2.set_xlabel("log(walks N)")
ax2.set_ylabel("log(runtime (ms))")
ax2.set_title("Parallel v.s. Serial WoS runtime with 1000 points")
plt.suptitle("Domain Sphere")
plt.savefig(f"{outputpath}/timeplot_sphere.png",bbox_inches='tight')


# data 1: same walk, different sample points
parallel_1 = [300.061,164.321,231.115,
            579.126,482.899,394.348,451.6,
            580.009,810.797]

serial_1 = [351.922,726.343,1047.18,1377.98,
          1681.22,2041.55,2524.35,2803.1,3110.2]


fig = plt.figure(figsize=(15,6))
ax1 = fig.add_subplot(121)

ax1.plot(sample_pts,serial_1,label='serial',marker='o',lw=2.)
ax1.plot(sample_pts,parallel_1,label='parallel',marker='P',lw=2.)
ax1.plot(sample_pts,sample_pts,label='O(M)',linestyle='--',color='k',lw=2)
ax1.set_xscale('log')
ax1.set_yscale('log')
ax1.legend()
ax1.set_xlabel("log(sample points M)")
ax1.set_ylabel("log(runtime (ms))")
ax1.set_title("Parallel v.s. Serial WoS runtime with 2000 walks")

# data2: same sample points, different walks

parallel_2 = [196.251,176.349,313.222,419.05,400.286,
            480.085,746.295,700.132,882.451]
serial_2 = [478.455,841.096,1763.32,1222.91,2186.18,2548.76,2882.61,
          3508.51,3906.54]

ax2 = fig.add_subplot(122)
ax2.plot(walk,serial_2,label='serial',marker='o',lw=2.)
ax2.plot(walk,parallel_2,label='parallel',marker='P',lw=2.)
ax2.plot(walk,walk,color='k',label='O(N)',linestyle='--',lw=2.)
ax2.set_xscale('log')
ax2.set_yscale('log')
ax2.legend()
ax2.set_xlabel("log(walks N)")
ax2.set_ylabel("log(runtime (ms))")
ax2.set_title("Parallel v.s. Serial WoS runtime with 1000 points")
plt.suptitle("Domain Link")
plt.savefig(f"{outputpath}/timeplot_link.png",bbox_inches='tight')

# data 1: same walk, different sample points
parallel_1 = [101.159,197.458,244.104,
            338.853,354.119,528.125,401.731,
            546.289,535.202]

serial_1 = [404.972,803.217,1202.58,1604.96,
          2005.95,2403.52,2802.9,3196.58,3600.47]


fig = plt.figure(figsize=(15,6))
ax1 = fig.add_subplot(121)

ax1.plot(sample_pts,serial_1,label='serial',marker='o',lw=2.,markersize=4.)
ax1.plot(sample_pts,parallel_1,label='parallel',marker='P',lw=2.,markersize=4.)
ax1.plot(sample_pts,sample_pts,label='O(M)',color='k',linestyle='--',lw=2)
ax1.set_xscale('log')
ax1.set_yscale('log')
ax1.legend()
ax1.set_xlabel("log(sample points M)")
ax1.set_ylabel("log(runtime (ms))")
ax1.set_title("Parallel v.s. Serial WoS runtime with 2000 walks")

# data2: same sample points, different walks

parallel_2 = [91.821,215.183,277.57,447.826,357.15,
            442.74,543.526,591.225,706.674]
serial_2 = [501.779,1003.15,1502.14,2004.23,2506.69,3004.22,3516.21,
          4008.46,4514.4]

ax2 = fig.add_subplot(122)
ax2.plot(walk,serial_2,label='serial',marker='o',lw=2.,markersize=4.)
ax2.plot(walk,walk,color='k',label='O(N)',linestyle='--',lw=2.)
ax2.plot(walk,parallel_2,label='parallel',marker='P',lw=2.,markersize=4.)
ax2.set_xscale('log')
ax2.set_yscale('log')
ax2.legend()
ax2.set_xlabel("log(walks N)")
ax2.set_ylabel("log(runtime (ms))")
ax2.set_title("Parallel v.s. Serial WoS runtime with 1000 points")
plt.suptitle("Domain Capsule")
plt.savefig(f"{outputpath}/timeplot_capsule.png",bbox_inches='tight')

