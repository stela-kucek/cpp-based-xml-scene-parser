# c-based-xml-scene-parser

Similar to my webgl2 projects, this project also creates graphics (image output) by communicating with the GPU and shading the defined vertices. However, this project does not utilize a graphics/animation specific framwework, but is written in plain C++.

This application loads a selected XML scene file, parses the information therein to obtain object specifications, and finally renders and outputs an image of the constructed scene.

The instructions on how to compile and use this application are given in the [details.txt](https://github.com/stela-kucek/cpp-based-xml-scene-parser/blob/b3013cf53e2e9828a66970d3f0150c6372b0c26e/details.txt) file.

There are 4 example scenes. After they are successfully read, parsed, and rendered, the images are stored in the `output` folder.

The results should look something like this:

Scene 1:

![example1 png](https://user-images.githubusercontent.com/18488581/164990387-0e06ef87-48f1-49ff-98d1-fd9db5b791bd.jpg)

Scene 2:

![example2 png](https://user-images.githubusercontent.com/18488581/164990403-7dbf8217-7458-46ce-ba64-e4bc1cfe41b2.jpg)

Scene 3:

![example3 png](https://user-images.githubusercontent.com/18488581/164990413-5f1c697f-72cd-42dc-b672-1d6525c096c8.jpg)

Scene 4:

![example4 png](https://user-images.githubusercontent.com/18488581/164990423-ac181302-3408-4ca2-b76d-7e2e9404819a.jpg)
