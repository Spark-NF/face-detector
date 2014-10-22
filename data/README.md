# Data folder

Folder containing data for the program to function. Also contains user data.

## XML files

Pre-teached HAAR algorithm results.

### haarcascade_frontalface_alt.xml
Used to detect faces on an image.

### haarcascade_eye_tree_eyeglasses.xml
Used to detect eyes on an image.

### haarcascade_eye.xml
Used to detect eyes on an image (if the other didn't succeed).

## CSV files

User data databases. Data is separated by semicolons ";".

### persons.csv
Contains the list of added persons, linked to their group.

### groups.csv
Contains the list of added groups.

### pictures.csv
Contains the list of added pictures, linked to their person.

## Original
Folder containing the original face images added to the program.

## Modified
Folder containing the modified faces. Modifications are:
* Rotation (to have eyes horizontally)
* Translation (to have all faces' eyes aligned at the same place)
* Cropping (to keep only the important part, 25% around the face)
* Scaling (to save space and improve analysis and teaching speed, scaled to 100x100)