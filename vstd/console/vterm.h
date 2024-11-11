/**
 * @brief Terminal text colors.
 * 
 * Represents various colors that can be used for terminal text.
 */
typedef enum VTERM_COLOR {
    VTERM_COLOR_BLACK,        /**< Black color. */
    VTERM_COLOR_BLUE,         /**< Blue color. */
    VTERM_COLOR_GREEN,        /**< Green color. */
    VTERM_COLOR_CYAN,         /**< Cyan color. */
    VTERM_COLOR_RED,          /**< Red color. */
    VTERM_COLOR_PURPLE,       /**< Purple color. */
    VTERM_COLOR_BROWN,        /**< Brown color. */
    VTERM_COLOR_LIGHTGRAY,    /**< Light gray color. */
    VTERM_COLOR_DARKGRAY,     /**< Dark gray color. */
    VTERM_COLOR_LIGHTBLUE,    /**< Light blue color. */
    VTERM_COLOR_LIGHTGREEN,   /**< Light green color. */
    VTERM_COLOR_LIGHTCYAN,    /**< Light cyan color. */
    VTERM_COLOR_LIGHTRED,     /**< Light red color. */
    VTERM_COLOR_LIGHTPURPLE,  /**< Light purple color. */
    VTERM_COLOR_YELLOW,       /**< Yellow color. */
    VTERM_COLOR_WHITE         /**< White color. */
} VTERM_COLOR;

/**
 * @brief Terminal text attributes.
 * 
 * Defines text attributes for the terminal output such as bold, italic, etc.
 */
typedef enum VTERM_ASCII {
    VTERM_ASCII_NORMAL,    /**< Normal text attribute (default). */
    VTERM_ASCII_BOLD,      /**< Bold text attribute. */
    VTERM_ASCII_ITALIC,    /**< Italic text attribute. */
    VTERM_ASCII_UNDERLINE  /**< Underlined text attribute. */
} VTERM_ASCII;

/**
 * @brief Sets the text color and attribute for the terminal.
 * 
 * This function configures the background and foreground color of the terminal text,
 * as well as applying a specific text attribute (e.g., bold, italic, underline).
 * It outputs the appropriate ANSI escape sequence to modify the terminal text appearance.
 * 
 * @param bg_color The background color (enum VTERM_COLOR).
 * @param fg_color The foreground color (enum VTERM_COLOR).
 * @param ascii    The text attribute (enum VTERM_ASCII), such as normal, bold, italic, or underline.
 */
void vterm_set_color(enum VTERM_COLOR bg_color, enum VTERM_COLOR fg_color, enum VTERM_ASCII ascii);

/**
 * @brief Resets the text color and attribute to default settings.
 * 
 * This function resets both the text color and text attribute of the terminal
 * to their default settings.
 */
void vterm_reset_color(void);

/**
 * @brief 2D vector for cursor positioning.
 * 
 * Represents a 2D coordinate with x and y values for positioning the terminal cursor.
 */
typedef struct VTERM_VEC {
    int x; /**< X-coordinate position. */
    int y; /**< Y-coordinate position. */
} VTERM_VEC;

/**
 * @brief Sets the position of the cursor on the terminal.
 * 
 * This function moves the terminal cursor to a specified (x, y) position.
 * 
 * @param x The X-coordinate of the cursor position.
 * @param y The Y-coordinate of the cursor position.
 */
void vterm_set_cursor_pos(int x, int y);

/**
 * @brief Gets the current cursor position.
 * 
 * This function returns the current position of the terminal cursor as a `VTERM_VEC` struct.
 * 
 * @return A struct containing the X and Y coordinates of the cursor.
 */
struct VTERM_VEC vterm_get_cursor_pos(void);

/**
 * @brief Gets the terminal's width and height.
 * 
 * This function returns the width and height of the terminal window.
 * 
 * @return A struct containing the width (X) and height (Y) of the terminal.
 */
struct VTERM_VEC vterm_get_width_height(void);
