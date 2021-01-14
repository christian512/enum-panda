# enum-panda

## About PANDA
This is a "fork" of the Parallel AdjaceNcy Decomposition Algorithm: PANDA. 
Details to PANDA can be found here:

Lörwald, S., Reinelt, G.: PANDA: a Software for Polyhedral Transformation (published in EURO Journal on Computational Optimization, 2015, DOI 10.1007/s13675-015-0040-0)

PANDA allows is a implementation of the Adjacency Decomposition method, to enumerate all facets/vertices of a polytope.
It processes input files of either the H- or V-representation of a polyhedron. In these input files, you can give symmetry
maps, that are used to determine only one class representative for all facets/vertices. 
In this way the computation of all unique classes can be speed up.

## Enum-PANDA
We are using the Panda library to find the vertices of the local polytope (under additional constraints) for specific
bell setups. However the functionality given by the symmetry maps, is not sufficient for the class equivalence that 
we need for the vertices/facets. 
Thus we will extend the class definition here, by an algorithm that we already outlined several times. 
(TODO: Add brief description of the algorithm here).

This algorithm is usually applied after finding the vertices. However if we already apply, during the vertex enumeration, 
we hope to get some faster results. 
Another point is, that the enumeration loops forever in some symmetry cases. We assume that this is due to the incomplete
equivalence check. 

## Setup of PANDA
To compile and install PANDA run the following commands.

```bash
cd build
cmake ..
make
ctest # optional if ctest is installed
make install
```

You can then call ``panda`` from any directory 