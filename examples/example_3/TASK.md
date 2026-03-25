# Vector2D implementation

1. Create a class called Vector2D that represents a 2d vector in space.
2. It will need a private name/label and a public x and y components.

## Base methods
### Constructors and Methods (aka functions of the class)

1. Add a custom constructor that takes (label, x, y). Also add the default one.
2. set_label(std::string new_label) method
3. set(double x, double y) to change the x and y values
4. print() const, to print the member values.

### CSV "Comma Separated Values" format

1. to_csv_heder() const, print the CSV header, (like: "label,x,y").
2. to_csv_row() const, print the values in a CSV format (""my_label", 0.0, -1.0"). How can you print the double quotes? Check online how to "escape" the double quotes (similar to what we did in class).
 
## Add Vector2D behavior

1. double norm() const, returns the Euclidean norm. (hint: look in cppreference at "std::hypot").
2. void scale(double factor), scales both x and y components of the same ammount.
3. void add(const Vector2D &other), modify my components by summing the other vector's components.

## std::vector usage
Implement this directly in the main() function.

1. Create a std::vector<Vector2D> with at least 3 elements.
2. Initialize them with different values
3. Print all elements using a loop
4. Scale all elements by a factor of 2 using a reference-based loop
5. Print them again to verify the change

Make a test by changing the loop from a reference based one (const Vector2D &element / Vector2D &element) to a copyied one (Vector2D element) and verify the change.

When printing the std::vector, try calling before the for loop the to_csv_header(), and inside the loop call to_csv_row(). Is the result a valid CSV formatted text? Can be imported for example in Matlab to plot the Vector2D's values?