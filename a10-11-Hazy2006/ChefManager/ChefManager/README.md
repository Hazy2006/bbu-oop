# Chef & Recipe Manager (Qt C++)

## Functionalities:
1. Load Data: Application loads Chefs and Recipes from text files at startup.
2. Display Windows: A distinct window opens for every chef in the database.
3. Sorting: Recipes are sorted alphabetically by cuisine upon display.
4. Add Recipe: Users can add recipes with validation (no empty names, no duplicates, valid integer times).
5. Specialty Filter: A checkbox filters the list to only show the specific chef's specialty cuisine.
6. Ingredient Search: A text bar filters recipes by comma-separated ingredients (space-insensitive, must contain all terms).
7. Graphical View: A button opens a secondary canvas window for that cuisine.
   - Shapes are drawn based on recipe properties (Even name length = Rectangle, Odd = Circle).
   - Dimensions scale dynamically based on prep time (bounded between 20px and 150px).
8. Observer Pattern: All open windows update instantly when new data is added.
9. Persistence: On application exit, data is sorted (Cuisine alphabetical -> Prep Time shortest) and saved back to the file.
