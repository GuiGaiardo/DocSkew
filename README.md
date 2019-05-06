# DocSkew

This is an algorithm that estimate the skew angle of a given document image using Mathematical Morphology and Hough Transform. The implementation uses OpenCV for reading and processing images.

## Requirements
-OpenCV

-GCC

-OpenMP (to enable multithreading)

## Compiling
g++ estimate.cpp -o estimate \`pkg-config --cflags --libs opencv\` -fopenmp

## Usage
In order to estimate the skew of a set of document images contained in a directory you can use the following command:
./estimate <directory_name> [visualize]

The [visualize] option enables the visualization of the pre-processed image and the found lines overlayed in the original image.

Examples:
1) ./estimate samples/ > results.txt
The above command will run the algorithm for all images contained in the 'samples' directory and write the results on the 'results.txt' file.
Later you can run the analyse.py script to collect the results.

2) ./estimate samples/ 1
The above command will function the same as the first example, but it will generate intermediate results images, in order to show the process in more detail. This will not generate an output that's interpretable by the analyse.py script.

## Collecting Results
To collect the results from running the (1) example command, you can use the analyse.py script. In order to collect the results you need a file with the ground truth for all tested images.

It will generate the following results:

-Average Absolute Error (AED)

-Averate Top 80 (AED for the 80% best estimations)

-No Corrects (No. of estimations with error <= 0.1 degrees)

-Correct Estimations (% of correct estimations)

-Max bottom20 (maximum error)

## Dataset
We refer the reader to the ICDAR-DISEC2013 dataset to test the algorithm. It can be found in http://users.iit.demokritos.gr/~alexpap/DISEC13/resources.html. The dataset contains 1550 real document images along their ground truth. The dataset contains a number of features, such as languages, contents, layouts, fonts...
