//@ref at https://stackoverflow.com/questions/4842424/list-of-ansi-color-escape-sequences


/**
 * @brief Console text colors.
 * 
 * Represents various colors that can be used for console text.
 */
typedef enum VCONSOLE_COLOR {
    VCONSOLE_COLOR_BLACK,        /**< Black color. */
    VCONSOLE_COLOR_BLUE,         /**< Blue color. */
    VCONSOLE_COLOR_GREEN,        /**< Green color. */
    VCONSOLE_COLOR_CYAN,         /**< Cyan color. */
    VCONSOLE_COLOR_RED,          /**< Red color. */
    VCONSOLE_COLOR_PURPLE,       /**< Purple color. */
    VCONSOLE_COLOR_BROWN,        /**< Brown color. */
    VCONSOLE_COLOR_LIGHTGRAY,    /**< Light gray color. */
    VCONSOLE_COLOR_DARKGRAY,     /**< Dark gray color. */
    VCONSOLE_COLOR_LIGHTBLUE,    /**< Light blue color. */
    VCONSOLE_COLOR_LIGHTGREEN,   /**< Light green color. */
    VCONSOLE_COLOR_LIGHTCYAN,    /**< Light cyan color. */
    VCONSOLE_COLOR_LIGHTRED,     /**< Light red color. */
    VCONSOLE_COLOR_LIGHTPURPLE,  /**< Light purple color. */
    VCONSOLE_COLOR_YELLOW,       /**< Yellow color. */
    VCONSOLE_COLOR_WHITE         /**< White color. */
} VCONSOLE_COLOR;

/**
 * @brief Console text attributes.
 * 
 * Defines text attributes for the console output such as bold, italic, etc.
 */
typedef enum VCONSOLE_ASCII {
    VCONSOLE_ASCII_NORMAL,    /**< Normal text attribute (default). */
    VCONSOLE_ASCII_BOLD,      /**< Bold text attribute. */
    VCONSOLE_ASCII_ITALIC,    /**< Italic text attribute. */
    VCONSOLE_ASCII_UNDERLINE  /**< Underlined text attribute. */
} VCONSOLE_ASCII;

/**
 * @brief Sets the text color and attribute for the console.
 * 
 * This function configures the background and foreground color of the console text,
 * as well as applying a specific text attribute (e.g., bold, italic, underline).
 * It outputs the appropriate ANSI escape sequence to modify the console text appearance.
 * 
 * @param type     The output stream type (enum VCONSOLE_TYPE), such as standard output or error.
 * @param bg_color The background color (enum VCONSOLE_COLOR).
 * @param fg_color The foreground color (enum VCONSOLE_COLOR).
 * @param ascii    The text attribute (enum VCONSOLE_ASCII), such as normal, bold, italic, or underline.
 */
void VConsole_SetColor(enum VCONSOLE_COLOR bg_color, enum VCONSOLE_COLOR fg_color, enum VCONSOLE_ASCII ascii);

/**
 * @brief Resets the text color and attribute to default settings.
 * 
 * This function resets both the text color and text attribute of the console
 * to their default settings.
 */
void VConsole_ResetColor(void);

/**
 * @brief 2D vector for cursor positioning.
 * 
 * Represents a 2D coordinate with x and y values for positioning the console cursor.
 */
typedef struct VCONSOLE_VEC {
    int x; /**< X-coordinate position. */
    int y; /**< Y-coordinate position. */
} VCONSOLE_VEC;

/**
 * @brief Sets the position of the cursor on the console.
 * 
 * This function moves the console cursor to a specified (x, y) position.
 * 
 * @param x The X-coordinate of the cursor position.
 * @param y The Y-coordinate of the cursor position.
 */
void VConsole_SetCursorPos(int x, int y);

/**
 * @brief Gets the current cursor position.
 * 
 * This function returns the current position of the console cursor as a `VCONSOLE_VEC` struct.
 * 
 * @return A struct containing the X and Y coordinates of the cursor.
 */
struct VCONSOLE_VEC VConsole_GetCursorPos(void);

/**
 * @brief Gets the console's width and height.
 * 
 * This function returns the width and height of the console window.
 * 
 * @return A struct containing the width (X) and height (Y) of the console.
 */
struct VCONSOLE_VEC VConsole_GetWidthHeight(void);
