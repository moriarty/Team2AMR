/* -*- mode: c++; -*- */

/** @file PSTermIOSimple.h Definitions for simple asynchronous terminal IO class. 

 (C) 2009-2010  Bjoern Kahl

*/

#ifndef PSTERMIOSIMPLE_H
#define PSTERMIOSIMPLE_H

#include <termios.h>
#include <stdexcept>
#include <string>
#include <vector>


namespace MobileRobots {


/** Asynchronous terminal IO.
 *
 * This class provides asynchronous (non-blocking) terminal IO.  It
 * assumes an ANSI / vt102 terminal type and uses no external
 * librarires except the standard Unix C-runtime and the C++ standard
 * library.  It works by polling the terminal using select() and
 * then assembling a line from available characters.  It can be used to
 * read full lines (i.e. a new-line terminated sequence of characters)
 * or to read single characters.  When in full-line mode, it supports
 * a limited set of line-editing operations.
 *
 * This class supports the following modes of operation:
 *
 * Full-line mode: A full line is collected.  Reading of individual
 * characters is not possible.
 *
 * Single-char mode: Characters are read one-by-one using getChar().
 *
 * @par Full-line mode:
 *
 * Full-line mode is implicitely activated by the requestLine()
 * function which prints a prompt and then waits for the user to type
 * a line and hit \<enter\>.  Of course, waiting is done in the
 * background, requestLine() returns immediately.
 *
 * While in full-line mode, the getChar() function is disabled and
 * always returns zero.
 *
 * Implicite full line mode ends automatically when the requested line
 * has been completed (i.e. user hits \<enter\>) or aborted (user hits
 * \<esc\> twice).  It can be manually ended by calling
 * cancelRequestLine().
 *
 * @par Single-char mode:
 *
 * While not in full-line mode, the getChar() function can be used to
 * read a single character.  Charcters not read by getChar() are discarded.
 *
 * @par Character echoing
 *
 * The echoing of type characters back to the terminal (i.e. showing
 * characters as they are typed) is enabled when in full line mode,
 * disabled otherwise.  Note that echoing in full-line mode is done by
 * the PSTermIO class, the terminal echo is always of.
 *
 *  @par Usage example:
 *
 *  @code
 *  PSTermIO term;
 *  const char *prompt1 = "What is your name ? ";
 *  std::string answer;
 *
 *  term.requestLine(prompt1);
 *  bool wantAnswer = true; // remember we want an answer
 *  for (;;) {
 *    // do something
 *
 *    // check for an answer
 *    if (term.getLine(answer)) {
 *      // process answer
 *      wantAnswer = false;
 *    }
 *    if (wantAnswer && !requestLinePending()) {
 *      // user canceled line input.  Do something about it
 *      wantAnswer = false;
 *    }
 *
 *    // do something
 *  }
 *  @endcode
 *
 *  @par Error reporting
 *
 *  Most functions do not return error codes, instead in case of an
 *  error an exception of type PSTermIOException is thrown.
 *
 *  @note The class claims the terminal at construction time and
 *  configures it to raw mode, thus disabling normal terminal IO.  You
 *  can still write to the terminal using libc's write() or the C
 *  standard lib printf() or cout familiy of functions, but no output
 *  post-processing will take place.  Especially this means that no
 *  automatic line feed on printing a CR (ASCII 13) occurs, nor a
 *  automatic carriage return on NL (ASCII 10).
 */
class PSTermIO {
public:
  /** Default constructor.
   *
   *  The PSTermIO class takes no arguments at construction time, it
   *  is always constructed by the default constructor.
   *
   *  @note The class claims the terminal at construction time and
   *  configures it to raw mode, thus disabling normal terminal IO.
   *  You can still write to the terminal using libc's write() or the
   *  C standard lib printf() or cout familiy of functions, but no
   *  output post-processing will take place.  Especially this means
   *  that no automatic line feed on printing a CR (ASCII 13) occurs,
   *  nor a automatic carriage return on NL (ASCII 10).
   */
  PSTermIO();

#ifdef DEBUG_PSTERMIO
  /** @internal Debug constructor.
   *
   *  This constructor is for debugging use.  It installs and a log
   *  file and initiates logging of terminal IO and various internla
   *  state changes to the log file.  Logfile is @em not closed at
   *  destruction time.
   *
   *  @param logfile_a : C FILE to log data to.  Must be open for
   *  writing.
   */
  PSTermIO(FILE *logfile_a);
#endif

  /* Destructor, restore terminal to initial state. */
  ~PSTermIO();

  /** Request the user to type-in a line of text.
   *
   *  This function displays the specified prompt and initiates
   *  collections of typed-in characters int a full line (a sequence
   *  of characters terminated by a newline character).  The prompt
   *  can be an empty string, a single line or any number of lines of
   *  text.  For a multi-line prompt, make sure you use "\n" as line
   *  separator, without any "\r" (that is, use only new-line
   *  characters and no carriage-return characters).  If the prompt
   *  does not end in a new-line, then requestLine() will start
   *  waiting for the to-be-read line just after the last character of
   *  the prompt.
   *
   *  Example:
   *  @code
   *  PSTermIO term;
   *  const char *prompt1 = "What is your name ? ";
   *
   *  term.requestLine(prompt1);
   *  @endcode
   *
   *  Will create this screen layout:
   *  @verbatim
   *
   *  What is your name ? #
   *
   *  @endverbatim
   *  Where "#" is the position of the cursor after return from
   *  requestLine(), while
   *  @code
   *  PSTermIO term;
   *  const char *prompt2 = "Hi, I am HAL-9000 \n WHat can I do for you? \n";
   *
   *  term.requestLine(prompt2);
   *  @endcode
   *
   *  Will create this screen layout:
   *  @verbatim
   *
   *  Hi, I am HAL-9000 
   *  WHat can I do for you? 
   *  #
   *
   *  @endverbatim
   *  Where "#" is the position of the cursor after return from
   *  requestLine().
   *
   *  The completed line can be read by calling getLine().  Calling
   *  the requestLine() function again while a privious call is
   *  pending is an error.  Any pending call to requestLine() can be
   *  canceled by calling cancelRequestLine().
   *
   *  Note that the class remembers the cursor position before writing
   *  the prompt to the terminal, and uses the remembered position to
   *  continue writing text at that postion with the print() function.
   *
   *  @param prompt : The text to display in front of the line to be
   *  read.  This can be an empty string, a single line or any number
   *  of text lines.  For multi-line prompt, make sure you use "\n" as
   *  line separator, without any "\r" (that is, use new-line
   *  characters and no carriage-return characters).
   */
  void requestLine(const char *prompt);

  /** Determine if a requestLine() call is pending.
   *
   *  @return This function returns true if a call to requestLine() is
   *  pending, false otherwise.
   */
  bool checkRequestPending(void);

  /** Read full line.
   *
   *  This function returns a complete line read from the terminal.
   *  If no complete line is available, it return false, other wise
   *  the line's text is copied into the supplied String object.
   *
   *  If no requestLine() call is active, then this function returns
   *  false and does not change @a dest.
   *
   *  @param dest A string object which will be filled with the line
   *  read from terminal.
   *
   *  @return flag if a line could be obtained.
   *
   *  @retval true A line was available and has been stored in @a dest.
   *
   *  @retval false No complete line was available.  The @a dest
   *  object was not modified.
   */
  bool getLine(std::string &dest);

  /**  Wait at most @a timeout milliseconds for a full line.
   *
   *  This function is a variant of getLine(void), see there for
   *  details.  It differs from getLine(void) by taking an additional
   *  parameter @a timeout, which specifies an alternate upper limit
   *  on the time waite for new characters to be typed in.
   *
   *  Note that this function does not wait until a line is
   *  completed.  Instead it returns as soons a single character has
   *  been added to the line, or a single control sequence (like
   *  cursor movement) has been processed.
   *
   *  To wait forever, sepcify a timeout of zero.  A timeout of -1 is
   *  equivalent to calling getLine(std::string &dest), i.e. do not
   *  wait at all.
   *
   *  @param dest A string object which will be filled with the line
   *  read from terminal.
   *
   *  @param timeout : Time to wait for new characters to arrive,
   *  specified in milliseconds.  The value of zero stands for
   *  infinity, blocking until atleast one character could be read.
   *  The value of -1 is equivalent to calling getLine(std::string
   *  &dest), i.e. does not wait at all.
   */
  bool getLine(std::string &dest, int timeout);

  /** Return incomplete line.
   *
   *  This function returns the characters typed so far in the current
   *  line.  If no current line exists (checkRequestPending() returns
   *  false), then a null string is returned.
   *
   *  The state of the line editing is not altered.  No characters are
   *  removed from the line currently typed in.
   *
   *  @return pointer to the null-terminated character string.  The
   *  string storage is stically allocated and only valid until the
   *  next call to getLine(), getChar() or update().
   */
  const char *peekLine(void);

  /** Cancel a pending requestLine() call. 
   *
   *  This function cancels a previous requestLine() call.  Any input
   *  collected sofar is discarded, the terminal line including prompt
   *  erased and the class returns to the single character mode.
   *
   *  A call to this function while no requestLine() is pending is
   *  silently ignored.
   */
  void cancelRequestLine(void);

  /** Read a single character.
   *
   *  This function tries to read a single character from the
   *  terminal.  It waits up to the default read timeout miliseconds
   *  (see setTimeout()) for input.  The class maintains an input
   *  buffer of length one.  If some characters have been typed in
   *  before the call to getChar(), then only the last one will be
   *  returned.  In that case no waiting for new input is done.
   *
   *  Note that some keys like the cursor keys generate more than one
   *  character. Such a multi character sequence generated by a single
   *  key-strok is returned by getChar() one-by-one, subsequent tped
   *  in characters do not overwrite the tail of a half-read
   *  multi-character sequence.  However at most one subsequent
   *  character will be stored, with new characters arriving
   *  overwriting earlier ones.
   *
   *  Example:
   *
   *  @code
   *  PSTermIO term;
   *
   *  // empty buffer
   *  while (term.getChar() != 0)
   *   ;  // empty loop, do nothing
   *
   *  // ... do sometghing ...
   *  // assume someone types "<cursor-left> a b" while we do something
   *
   *  char c = term.getChar();  // getChar() will return the "b"
   *
   *  // empty buffer
   *  while (term.getChar() != 0)
   *   ;  // empty loop, do nothing
   *
   *  // ... do sometghing ...
   *  // someone pressed the <cursor-left> key
   *
   *  c = term.getChar();  // getChar() returns first byte from <cursor-left> sequence
   *
   *  // ... do sometghing ...
   *  // someone typed "a b" while we do something
   *
   *  c = term.getChar();  // getChar() returns next byte from <cursor-left>
   *  // ... read rest of <cursor-left>
   *  c = term.getChar();  // getChar() returns "b", which overwrote the waiting "a".
   *  @endcode
   *
   *  @return The function returns the read character or zero if no
   *  character is available.
   *
   *  See also paragraph on full-line mode in the class PSTermIO
   *  documentation.
   */
  char getChar(void);

  /** Wait at most @a timeout milliseconds for a character.
   *
   *  This function is a variant of getChar(void), see there for
   *  details.  It differs from getChar(void) by taking an additional
   *  parameter @a timeout, which specifies an alternate upper limit
   *  on the time waite for a character to be typed in.  Note that the
   *  function returns as soon as some input was available.
   *
   *  To wait forever, sepcify a timeout of zero.  A timeout of -1 is
   *  equivalent to calling getChar(void), i.e. do not wait at all.
   *
   *  @param timeout : Time to wait for new character to arrive,
   *  specified in milliseconds.  The value of zero stands for
   *  infinity, blocking until atleast one character could be read.
   *  The value of -1 is equivalent to calling getChar(void), i.e. does
   *  not wait at all.
   *
   *  @sa getChar(void);
   */
  char getChar(int timeout);

  /** Print text without disturbing line input.
   *
   *  This function prints text, without overwriting any active line
   *  editing for requestLine().  The text will start at the same
   *  cursor position where the text from the last call to print()
   *  ended, or where the cursor was at the last call to
   *  requestLine().  The current prompt and line edited will be
   *  erased, the text output and the prompt and edited line reprinted
   *  following the the text from print().
   *
   *  @note The last position remembered will be reset when the user
   *  completes a line input by hiting \<enter\>.  In this case the next
   *  printing position is set to the first column of the row
   *  following the just completed line of input.
   *
   *  If a requestLine() is canceled, the printing position is @em not
   *  reset, instead the propmt and canceled input line are erased and
   *  the cursor is positioned at the end position of the last call to
   *  print().
   *
   *  @param text : The text to be printed.  This can be a multi-line
   *  text and does not need to end with a newline.  The text is
   *  output where the previous call to print() stopped or where the
   *  cursor was at the last call to requestLine().
   */
  void print(const char*text);

  /** Print formated text without disturbing input line.
   *
   *  This function is a variation of print(const char*txt).  See
   *  there for details.  It differes from print(const char*txt) by
   *  taking a format string @afmt in the usual printf style and
   *  additional arguments to be formated by @a fmt.
   *
   *  @param fmt Format string as specified for the C standard lib
   *  function fprintf().
   *
   *  @sa print(const char*text)
   */
  void printf(const char *fmt, ...);

  /** Process any pending input.
   *
   *  This function tries to read new input from the terminal and
   *  process it into a complete line.  It also updates the visual
   *  representation on screen.  This function does not wait.
   *
   *  This function or the update(int timeout) variant should be
   *  regularly called with no more then 100 ms between two calls.  If
   *  getChar() or getLine() is called regularly (10 or more times a
   *  second), then no update() function needs to be called, it will
   *  be automatically called by getChar() or getLine() as needed.
   *
   *  This function does not block, if no input is available, it
   *  returns immediately, ignoring the timeout set with setTimeout().
   */
  void update(void);

  /** Wait for input and process it.
   *
   *  This function is a variant of update(void), see there for
   *  details.  It differs from update(void) by an additional
   *  parameter allowing to specify an upper limit on the time waited
   *  for new input.  Note that the function returns as soon as some
   *  input was available.
   *
   *  To wait forever, sepcify a timeout of zero.  A timeout of -1 is
   *  equivalent to calling update(void), i.e. do not wait at all.
   *
   *  @param timeout : Max time to wait for new input, specified in
   *  milliseconds.  The value of zero stands for infinity, blocking
   *  iuntil atleast one character could be read.  The value of -1 is
   *  equivalent to calling update(void), i.e. does not wait at all.
   *
   *  @sa update(void)
   */
  void update(int timeout);

  /** Set default read timeout.
   *
   *  This function set the upper limit for the time waited for new
   *  characters when getLine() or getChar() is called.  Note that
   *  update(void) does not use this timeout, as documented there.
   *
   *  The @a milliseconds parameter supports two special values: -1
   *  disables waiting at all, and 0 enables blocking mode, waiting
   *  forever until at least one character could be read.  Note that
   *  blocking mode does not guarantee the getLine() will always
   *  return a new line.  Instead, getLine() will return as soon as at
   *  least one character could be added to the line currently
   *  assembled.  Similar, even if in blocking mode, getChar() will
   *  return imediately if the input buffer still holds unreported
   *  characters.
   *
   *  @param milliseconds : Default wait tiem in milliseconds.  if set
   *  to zero, each call to getChar() or getLine will block until at
   *  least one character could be read.  If set to -1, no waiting
   *  will be done.
   */
  void setTimeout(int milliseconds);

  /** Make terminal beep. */
  void beep(void);

  /** Get size of display.
   *
   *  This function returns the PSTermIO's idea of screen size.  This
   *  is the size in character rows and columns at the time of class
   *  instantiation.  Subsequent changes in window size may or may not
   *  be refelcted, depending on the capabilities of the current
   *  terminal (i.e. does the terminal report size changes nor not).
   *  A refresh of PSTermIO's idea of screen size can be forced by
   *  calling termioReinitState().  The current screen size is
   *  returned in the two result parameters.
   *
   *  @param numRows Return parameter, will be set to PSTermiIO's
   *  current assumption of number of visible character rows on the
   *  screen.
   *
   *  @param numCols Return parameter, will be set to PSTermiIO's
   *  current assumption of number of visible character columns on the
   *  screen.
   */
  void getScreenSize(unsigned int &numRows, unsigned int &numCols);

  /** Reinitialize terminal.
   *
   *  Low-level maintenace function, forces PSTermIO to forget its
   *  internal steate and re-initialize the terminal.  This involves a
   *  clear screen and repaint of active input.  It also re-reads the
   *  physical terminal size.
   */
  void termioReinitState(void);

  /** Clear screen.
   *
   *  This function clear the visible screen.  It should @em not be
   *  called while a requestLine() is active.  The interaction with an
   *  active requestLine() is undefined.
   */
  void clrScreen(void);

  /** Move cursor to upper-left corner.
   *
   *  This function moves the cursor to the upper-left corner of the
   *  screen.  It should @em not be called while a requestLine() is
   *  active.  The interaction with an active requestLine() is
   *  undefined.
   */
  void cursorHome(void);

  /** Move cursor to given position.
   *
   *  This function moves the cursor to position @a row and @a col.
   *  The next output using this class' print function or any other
   *  method to write characters to the screen will start output at
   *  the specified position.
   *
   *  @note Calling this function while a requestLine() is active has
   *  undefined effects.
   *
   *  @note This function uses a 1-based coordinate system, upper-left
   *  corner is (1;1).
   */
  void cursorSet(unsigned int row, unsigned int col);

  /** Read current cursor position.
   *
   *  @param row Return parameter, will be set to current cursor
   *  position row.
   *
   *  @param numCols Return parameter, will be set to current cursor
   *  position column.
   *
   *  @note This function uses a 1-based coordinate system, upper-left
   *  corner is (1;1).
   */
  void cursorGet(unsigned int &row, unsigned int &col);

  /** Error conditions reported by the class.
   *
   *  The enumeration list the error condition reported using objects
   *  of type PSTermIOException.  See individual member function
   *  documentation to learn which function throws which exception.
   */
  enum Error {
    /** Generic error of unknown cause. */
    Err_Unknown,

    /** IO error to terminal.
     *
     *  A read or write to the terminal failed permanently.  The
     *  exception object may have recorded the detailed error cause as
     *  part of the error description (see PSTermIOException::what())
     *  or the libc error code.
     */
    Err_IO,

    /** Calling requestLine() while another call is active. */
    Err_Busy,

    /** An undefined value was passed to an enum parameter. */
    Err_IllMode,

    /** Internal buffer overflow, due to out-of-range parameters to
     *  cursor movement or erase function.
     */
    Err_BufferOverflow,

    /** Value out of range.
     *
     *  The parameter to some function had a to small or to large
     *  value.
     */
    Err_Range
  };

private:
  /** Disabled Copy-Constructor. */
  PSTermIO(const PSTermIO &old);

  /** Disabled assignment operator. */
  PSTermIO &operator=(const PSTermIO &old);


  /* Here the internal terminal maintenance functions follow. */

  /** Initialize terminal and determin its capabilities. */
  void initTerm(void);

  /** Enable raw data mode for terminal.
   *
   *  In raw mode, no input preprocessing takes place and automatic
   *  echoing is off.  Required for the line editing functions of
   *  PSTermIO.
   */
  void makeTermRaw(void);

  /** Enable cooked data mode for terminal.
   *
   *  In cooked mode, the terminal driver does input pre-processing
   *  and output post-processing, including simple line editing
   *  (i.e. ony backspace is interpreted).  Incompatible for the line
   *  editing modes of PSTermIO.
   */
  void makeTermCooked(void);

  /** Enable or disable terminal-level input echoing. 
   *
   *  @param f : If ture, enable echoing, if false disable it.
   */
  void termSetEcho(bool f);

  /** Read current cursor position from terminal.
   *
   *  The position is returned in the result arguments.  The
   *  coordinate system is 1-based, i.e. (1;1) is the upper-left
   *  corner of the screen.
   *
   *  @param col : return parameter, will be set to column number.
   *
   *  @param row : return parameter, will be set to row number.
   *
   *  @return This function return true if the position could be read
   *  and false if an error occured.  In case of error the return
   *  parameters are not touched.
   */
  bool termGetPos(unsigned int &row, unsigned int &col);

  /** Set terminal cursor position.
   *
   *  The coordinate system is 1-based, i.e. (1;1) is the upper-left
   *  corner of the screen.
   *
   *  @param col : the new cursor column number.
   *
   *  @param row : the new cursor row number.
   *
   *  @return currently, this function always returns true on success.
   *  In case of an IO error, an exception of type PSTermIOException
   *  is thrown.
   */
  bool termSetPos(unsigned int row, unsigned int col);

  /** Definitions for termMove(). */
  enum MoveDir {
    /** Move cursor to the left. */
    MoveLeft,

    /** Move cursor to the right. */
    MoveRight,

    /** Move cursor up. */
    MoveUp,

    /** Move cursor down. */
    MoveDown};

  /** Scroll display up.
   *
   *  Ths function scrolls the terminal content by @a steps lines up,
   *  creating empty space at the bottom of the terminal.
   *
   *  After execution of this function, the cursor is in the
   *  bottom-left corner of the terminal.
   *
   *  @param steps Number of rows to scroll.
   */
  void termScrollUp(unsigned int steps);

  /** Move cursor around. 
   *
   *  This function moves the cursor by @a steps in the direction
   *  given by @a dir.
   *
   *  @param steps : number of rows or columns to move.  Note that it
   *  might not be possible to move accross the left or right border,
   *  while it is generally possible to move beyond the bottom border,
   *  effectively scrolling up.
   *
   *  @param dir : direction to move into, see MoveDir.
   */
  void termMove(unsigned int steps, MoveDir dir);

  /** Move cursor in logical line.
   *
   *  This function moves the cursor in the logical line (spanning
   *  more than one terminal row).  It updates curLineRow ans
   *  curLineCol.  If an attemp is made to move outside the editable
   *  area of the line, this function calls beep() and clips the
   *  movement to the allowed range.
   *
   *  @param steps : number of rows or columns to move.  This function
   *  can move the cursor across the left or right border, efectively
   *  jumping to the other side of the terminal and simultaneously
   *  moving up or down one line respectively.
   *
   *  @param dir : direction to move into, see MoveDir.
   */
  void lineMove(unsigned int steps, MoveDir dir);

  /** Set cursor to column relative to any defined prompt.
   *
   *  This functio sets the cursor to column @a point @em after any
   *  define prompt.  It correctly handles logical lines longer then a
   *  physical line, by line-wrapping the logical line as needed.
   *
   *  Note that point is zero-based, so @a point = 0 places the cursor
   *  at the beginning of the edit able are of the line, that is
   *  immediately after a defined prompt.  Setting @a point = logical
   *  line length moves the cursor past the last character currently
   *  in the line.
   *
   *  @param point : position is line to move to.  Note that point is
   *  zero-based.
   */
  void setCursorLinePoint(unsigned int point);

  /** Calculate row and column coordinates of point.
   *
   *  This function calculates the zero-based coordinates of @a point.
   *  The returned coordinates take a non-zero promptOffset into
   *  account, but not promptEndRow.  So if point is in the first row
   *  of the line (for example point = 0), then @a row will be zero
   *  independent of promptEndRow.
   *
   *  @param point : character position in current (logical) line
   *  which will be converted into coordinates.
   *
   *  @param row : result parameter, will be set to row number
   *  relative to promptEndRow where @a point is located.
   *
   *  @param col : result parameter, will be set to column number
   *  relative to left border of terminal (@em not relative to the
   *  column given by promptOffset).
   *
   *  Note: The coordinates are zero-based, (0;0) indicates upper-left
   *  corner.
   */
  void point2Pos(unsigned int point, unsigned int &row, unsigned int &col);

  /** Calculate character poistion in logical line.
   *
   *  This function is the inverse of point2Pos(unsigned int point,
   *  unsigned int &row, unsigned int &col), it takes zero-based
   *  row/column coordnates at determines the character position, that
   *  is the number of characters between the given coordinates ant
   *  the beginning of the editable area of the line.  This function
   *  takes promptOffset into account, but not promptEndRow, that is
   *  @a row is relative to the logiacl line, not to the terminal.
   *
   *  Example: Assume a (tiny) terminal with 10 columns per row and
   *  the prompt "Hi? " and the current line content "Hallo there!".
   *  The position of the cursor (3;3) is indicated by "#":
   *  @verbatim
   *
   *   1234567890
   * 1
   * 2 Hi? Hello 
   * 3 th#re!
   * 4
   *  @endverbatim 
   *
   *  Here, row would be 1 (second row of the line, but row counts
   *  from zero) and col would be 2 (third column, but col counts from
   *  zero).  The returned value in this example is then point = 12,
   *  the number of characters or steps to move from the beginning of
   *  the editable are ( which is(2;5) in the example in 1-based
   *  terminal coordinates) to the indicated coordinates.
   *
   *  @param row zero-based row number in the logical line.
   *
   *  @param col zero-based column number in the logical line.
   *
   *  @param point return value, set to the number of characters
   *  between start of logical ine and given coordinates.  Zero-based.
   */
  void pos2Point(unsigned int row, unsigned int col, unsigned int &point);

  /** Update curLineRow and curLineCol to coordinates reflecting @a
   *  point.
   *
   *  See discussion of point2Pos(unsigned int point, unsigned int
   *  &row, unsigned int &col) for details, but note that curLineRow
   *  and curLineCol are 1-based, while the result from point2Pos is
   *  zero-based.
   *
   *  @param point : character position in current (logical) line
   *  which will be converted into coordinates.
   */
  void recalcLineRowCol(unsigned int point);

  /** Correct wrong cursor position on some terminals after a line
   *  wrap occured.
   *
   *  Some terminals do not move the cursor in the next row, first
   *  column, when printing a character to the last column of a row.
   *  This function checks for this problem and repositions the cursor
   *  as needed.  Also takes care of lost scroll-up due to the same
   *  misbehavior of the terminal.
   */
  void fixBrokenLineWrap(void);

  /** Erase line modes.  See termEraseRow() for details. */
  enum ELMode {ELM_End, ELM_Start, ELM_All};

  /** Erase current terminal row.
   *
   *  This function erases text from the current row.  The text to be
   *  erased is selected by the mode.
   *
   *  @param mode : What part of the row to erase.  For ELM_Start
   *  erase text between start of row and cursor, for ELM_End erase
   *  text betwen end of row and cursor and for ELM_ALL erase all
   *  text in the row.
   */
  void termEraseRow(ELMode mode);


  /** Erase characters from current cursor position. 
   *
   *  This function erases @a num caharacter starting from current
   *  cursor position and moving to the right.  The characters are
   *  erased, not deleted, that is replaced with space characters.
   *  Character to the right of the erased area are not shifted to the
   *  left.
   *
   *  @param num Number of characers to erase.
   */
  void termEraseChars(unsigned int num);

  /** Convenient function for termEraseChars(ELMode mode).
   *
   *  This function is a variant of termEraseChars(ELMode mode), see there
   *  for details.  It differes from mentioned function by expilictly
   *  specifying the start position of the erased area.
   *
   *  @param num : number of chrarcters to be erased.
   *
   *  @param row : row to start erasing in.  Row is 1-based.
   *
   *  @param col : column in @a row to start erasing, moving right
   *  from here.  Column is 1-based.
   *
   *  Note : the coordinates @a row, @a col are 1-based, with (1;1)
   *  addressing the upper-left corner.
   */
  void termEraseChars(unsigned int num, unsigned int row, unsigned int col);

  /** Read new chars from terminal.
   *
   *  This function read new characters from the terminal, waiting at
   *  most @a timeout miliseconds.  Note that this function return as
   *  soon as any characters are available, so the actual time waited
   *  can be much shorter.  The red chars are stored in our internal
   *  input queue.
   *
   *  @param timeout : Maximum time in miliseconds to wait for new
   *  data.  This is an upper limit on the time waited, the actual
   *  time waited can be much shorter.  Set to zero to wait forever.
   *
   *  @param compactQueue : If set to false, do not remove already
   *  processed characters at start of inQueue and do not alter
   *  queueNext.  Otherwise move unprocessed content on inQueue to its
   *  beginning and set queueNext to zero accordingly.
   *
   *  @return This function return true if any data could be read.
   *  Note that a return value of false does not necessarily mean
   *  there is no data in inQueue.  It just means no data has been
   *  added by the cal to readTerm().
   */
  bool readTerm(int timeout, bool compactQueue = true);

  /** Erase and redraw current logical line on the terminal. */
  void redrawLine(void);

  /** Redraw prompt and line from requestLine().
   *
   *  This function erases and redraws the current prompt from
   *  requestLine() and content of the edited line.  It does nothing
   *  if no requestLine() is active.
   *
   *  The prompt will be arranged such that the first character of the
   *  dited line will be in the given row @a newPromptEndRow, which is
   *  zero-based.
   *
   *  @param newPromptEndRow The terminal row where the editable area
   *  of the logicalline should start.  This is zero-based.
   */
  void redrawPromptLine(unsigned int newPromptEndRow);

  /** Delete character at point.
   *
   *  This function deletes the character at @a point from the
   *  terminal, shifting subsequence characters one position to the
   *  left.  This function interprets @a point relativ to any defined
   *  prompt.
   *
   *  @note This function deals with @em logical lines.  Therefore it
   *  may erase or redraw more then one physical row on the terminal.
   *
   *  @param point : character position in current (logical) line
   *  where a character will be deleted.  Zero-based.
   */
  void termDeleteLinePoint(unsigned int point);

  /** Erase line on terminal, taking any defined prompt into account.
   *
   *  This function erases the terminal line from @a point to end of
   *  line (if @a mode = ELM_End) or to beginning of line (if @a mode
   *  = ELM_Start), where beginning of line is defined as first column
   *  @em after any defined prompt.  If @a mode = ELM_All, then the
   *  whole line (starting @em after any defined prompt) is erased.
   *
   *  @note This function deals with @em logical lines.  Therefore it
   *  may erase more then one physical row on the terminal.
   *
   *  @param mode : Direction in which to erase characters.
   *
   *  @param point : character position in current (logical) line
   *  from / to where the logical line will be erased.  Zero-based.
   */
  void termEraseLinePoint(unsigned int point, ELMode mode);



  /* Functions dealing with line editing. */

  /** Edit state in composeLine(). 
   *
   *  These flags control the processing in composeLine() and its
   *  backends appendChars(), insertChars() ans processCtrl().  They
   *  define waht should be done next, i.e. what type of input is
   *  currently processed / expected next.
   */
  enum EditState {
    /** Next character is neither a control character nor line end. */
    ES_Next,

    /** newline found, ende current line. */
    ES_End,

    /** All input has been processed, nothing more to do. */
    ES_Done,

    /** Next character is a control character and should be processed
	by processCtrl(). */
    ES_Ctrl,

    /** We run out of data while processing a multi-byte control
	sequence.  Nothing more to do. */
    ES_Incomplete
  };

  /** construct line from input characters.
   *
   *  This function is the main function doing the actual work of
   *  constructing a line from a sequence of input characters,
   *  including line edit commands.  It processes the input characters
   *  and dipatches them to insertChars() and appendChars() which deal
   *  with the visual terminal update.
   */
  void composeLine(void);

  /** Append some read chars to the stored line.
   *
   *  This function read the internal input queue and appends the found
   *  characters to the currently assembled line.  It also arranges
   *  for the terminal line to be updated accordingly.
   *
   *  This function updates curLineRow, curLineCol, insertPoint and
   *  the inQueue varibales queueNext and queueLen.
   *
   *  @return This function returns the new edit state.
   *
   *  @retval ES_Done : reached end of inQueue.
   *
   *  @retval ES_Ctrl : first unprocessed character in inQueue is a
   *  control character.
   */
  EditState appendChars(void);

  /** Insert read characters.
   *
   *  This function insert a sequence of characters from the internal
   *  input queue into the to-be-collected line.  It is used to @em
   *  insert characters somewhere in the middle or at the beginning of
   *  the line, as oposed to @em appending characters at the end.  It
   *  also arranges for the terminal line to be updated accordingly.
   *
   *  This function updates curLineRow, curLineCol, insertPoint and
   *  the inQueue varibales queueNext and queueLen.
   *
   *  @return This function returns the new edit state.
   *
   *  @retval ES_Done : reached end of inQueue.
   *
   *  @retval ES_Ctrl : first unprocessed character in inQueue is a
   *  control character.
   */
  EditState insertChars(void);

  /** Internal service routine for insertChars(void).
   *
   *  This function inserts @a dataLen bytes (must be printable
   *  characters, must not contain control charaters) from @a data
   *  into the currently edited line, at the position given by
   *  insertPoint.
   *
   *  This function updates curLineRow, curLineCol and insertPoint.
   *  It updates the screen display accordingly.
   *
   *  @param data Text to be inserted at the position given by
   *  insertPoint.
   *
   *  @param dataLen Number of characters to insert.
   *
   *  @note The insertPoint variable must point to a valid character
   *  position and be consistent with the current terminal cursor
   *  coordinates.
   */
  void insertCharsIntern(const char *data, int dataLen);

  /** Process control sequence.
   *
   *  This function process all control sequences like cursor movement
   *  understood by this class.  The following sequences are
   *  implemented: cursor left, cursor right, Ctrl-A, Ctrl-E, Ctrl-K
   *  (kill to end of line), Ctrl-Y (insert at point), Ctrl-L (redraw
   *  line), Del, BS.
   *
   *  This function expects queueNext to point either at a single-char
   *  control character or at the first character of a multi byte
   *  control sequence.
   *
   *  This function updates curLineRow, curLineCol, insertPoint and
   *  the inQueue varibales queueNext and queueLen.
   *
   *  @return This function returns the new edit state.
   *
   *  @retval ES_Done : reached end of inQueue.
   *
   *  @retval ES_Incomplete : reached end of inQueue while parsing a
   *  multi byte control sequence.
   *
   *  @retval ES_Next : first unprocessed character in inQueue is a
   *  non-control character.
   *
   *  @retval ES_End : The line has been completed by pressing \<enter\>.
   */
  EditState processCtrl(void);

  /** Internal helper for processCtrl.
   *
   *  This function deals with all multi-byte control sequences
   *  introduced by the ESC (0x1b) character.  It again delegates the
   *  parsing of the Esc and CSI sequences to parseCtrlSequence(),
   *  which in turn uses parseCSISequence().
   *
   *  This function expects queueNext to point at the first character
   *  of a multi byte control sequence, ie at an ESC or CSI character.
   *
   *  This function updates curLineRow, curLineCol, insertPoint and
   *  the inQueue varibales queueNext and queueLen.
   *
   *  @return This function returns the new edit state.
   *
   *  @retval ES_Done : reached end of inQueue.
   *
   *  @retval ES_Incomplete : reached end of inQueue while parsing a
   *  multi byte control sequence.
   *
   *  @retval ES_Next : first unprocessed character in inQueue is a
   *  non-control character.
   */
  EditState processEscCtrl(void);

#ifdef DEBUG_PSTERMIO
public:  // For debugging, allow acces to the ctrlInfo structure.
#endif

  /** Description of a control sequence or character.
   *
   *  This structure is filled in by parseCtrlSequence() or
   *  parseCSISequence().
   */
  struct ctrlInfo {
    ctrlInfo();  ///< Default constructor.
    ~ctrlInfo();  ///< Default destructor.

    /** Convert type value to string.
     *
     *  This function returns a printable representation of
     *  ctrlInfo::type.
     *
     *  @return pointer to string corresponding to the value of
     *  ctrlInfo::type.  The string is statically allocated.
     */
    const char *type2Str(void) const;

    /** Convert subtype value to string.
     *
     *  This function returns a printable representation of
     *  ctrlInfo::subtype.
     *
     *  @return pointer to string corresponding to the value of
     *  ctrlInfo::subtype.  The string is statically allocated.
     */
    const char *subtype2Str(void) const;

    /** start offset of this ctrl sequence.  A copy of the value
     *  supplied in the first parameter to parseCtrlSequence(). */
    unsigned int start;

    /** End offset of this ctrl sequence.
     *
     *  That is the offest of the first byte not belonging to the
     *	sequence.  Undefined if the sequence is incomplete.
     */
    unsigned int end;

    /** the command byte for the sequence.
     *
     *  For single-byte sequences, this is the printable character
     *  which cretaes the found control character if pressed with the
     *  Ctrl key hold.  Example: \<enter\> (newline, hex 0x0a) will be
     *  reported as "j", because Ctrl-j creates a new line character.
     *
     *  Single-byte sequence is indicated by ctrlInfo::type being set
     *  to Ctrl.
     *
     *  For multi-byte sequences, this is the ESC or CSI command
     *  character, usually the last character in the sequence.
     *
     *  Multi-byte sequence is indicated by ctrlInfo::type being set
     *  to Esc or CSI.
     */
    int cmd;  // signed because constructed from signed char.

    enum CIType {Undef, Ctrl, Esc, CSI};

    /** Type of control sequence.  See ctrlInfo::cmd for details. */
    CIType type;

    /** Type of Esc / CSI sequence.
     *
     *  This enumeration lists some of the more often needed multi
     *  byte sequences.  The function parseCSISequence() recognizes
     *  these codes and reports them in ctrlInfo::subtype, in addition
     *  to the usual reporting in ctrlInfo::cmd, ctrlInfo::type and
     *  the parameter array ctrlInfo::param.
     */
    enum CISubType {None, Fn, Left, Right, Up, Down, Home, End, PageUp, PageDown, PosReport};

    /** Subtype for Esc / CSI sequences, see CSISubType for
     *  details. */
    CISubType subtype;

    /** Number of parameters stored in ctrlInfo::param. */
    unsigned int paramCnt;

    /** Parameter array for CSI sequences.
     *
     *  The parameter are stored in the order they appear in the
     *  sequence.  The number of recognized parameters can be found in
     *  paramCnt.
     */
    unsigned int param[16];

    /** Flag, true if the sequence is complete.
     *
     *  For an incomplete sequence, only ctrlInfo::start,
     *  ctrlInfo::complete and ctrlInfo::type (which is set to Undef)
     *  are defined.
     */
    bool complete;

#ifdef DEBUG_PSTERMIO
    /* Debugging related stuff. */

    /** @internal Dump structure to log file. */
    void dump(FILE *logfile, const char *head=0) const;

    /** @internal Dump structure to log file. */
    void dump(const char *head=0) const;

    /** @internal C standard lib log file, must be open for writing. */
    static FILE *static_logfile;
#endif
  };

#ifdef DEBUG_PSTERMIO
private:
#endif

  /** Check if @a chr introduces a know control sequence.
   *
   *  This function checks if @a chr introduces a know control
   *  sequence.  Note that unknown single character control codes are
   *  not recognized, while multi-character control sequences are all
   *  recognized, independent of if the PSTermIO class knows how to
   *  handle them.
   *
   *  @param chr : character code to test.
   *
   *  @return This function return true if @a chr introduces a know
   *  control sequence, subject to above documented restriction.
   */
  bool isCtrlIntro(int chr);

  /** check control sequence, but do not execute it.
   *
   *  This function reads a control sequence and stores it in the
   *  supplied ctrlInfo structure @a ci.  The @a ci object must be
   *  allocated, but does not need to be initialized.  Most of the
   *  actual work is delegated to parseCSISequence().
   *
   *  This function does @em not touch any of curLineRow, curLineCol,
   *  insertPoint or the inQueue varibales queueNext and queueLen.
   *
   *  @param start : index (relative to queueNext) of first byte of
   *  the sequence, i.e. index of the control sequence introducer,
   *  usually the ESC (0x1b) character or for single character
   *  sequences index of the control code itself.
   *
   *  @param ci : result parameter.  The supplied ctrlInfo structure
   *  is filled out by this function.
   *
   *  @return This function returns the new edit state.
   *
   *  @retval ES_Done : reached end of inQueue, but obtained a
   *  complete control sequence.  The @a ci is valid, ci.end points at
   *  first character past the sequence (and past data inQueue)
   *
   *  @retval ES_Incomplete : reached end of inQueue while parsing a
   *  multi byte control sequence.  The @a ci is invalid, ci.end
   *  undefined.
   *
   *  @retval ES_Next : first unprocessed character in inQueue is a
   *  non-control character.  The @a ci is valid, ci.end points to the
   *  first unprocessed character in inQueue just past the completed
   *  control sequence.
   *
   *  @retval ES_Ctrl : first unprocessed character in inQueue is a
   *  control character.  The @a ci is valid, ci.end points to the
   *  first unprocessed character in inQueue just past the completed
   *  control sequence.
   */
  EditState parseCtrlSequence(unsigned int start, struct ctrlInfo &ci);

  /** internal helper to read CSI parameter sequences.
   *
   *  This function breaks a ECMA-48 CSI sequence into its numerical
   *  parameters and stores parameters and command code in the
   *  supplied ctrlInfo structure @a ci.  The @a ci must be
   *  pre-initialized, the following members must be set: ci.start,
   *  ci.complete must be false, ci.type must be ctrlInfo::CSI and
   *  ci.subtype must be ctrlInfo::None.
   *
   *  This function does @em not touch any of curLineRow, curLineCol,
   *  insertPoint or the inQueue varibales queueNext and queueLen.
   *
   *  @param start : index (relative to queueNext) of first byte of
   *  parameter set.  I.e. @em not the start of the whole sequence!
   *
   *  @param ci : result parameter.  The supplied ctrlInfo structure
   *  is filled out by this function.  See above for required
   *  pre-initialization.
   *
   *  @return This function returns the new edit state.
   *
   *  @retval ES_Done : reached end of inQueue, but obtained a
   *  complete control sequence.  The @a ci is valid, ci.end points at
   *  first character past the sequence (and past data inQueue)
   *
   *  @retval ES_Incomplete : reached end of inQueue while parsing a
   *  multi byte control sequence.  The @a ci is invalid, ci.end
   *  undefined.
   *
   *  @retval ES_Next : first unprocessed character in inQueue is a
   *  non-control character.  The @a ci is valid, ci.end points to the
   *  first unprocessed character in inQueue just past the completed
   *  control sequence.
   *
   *  @retval ES_Ctrl : first unprocessed character in inQueue is a
   *  control character.  The @a ci is valid, ci.end points to the
   *  first unprocessed character in inQueue just past the completed
   *  control sequence.
   */
  EditState parseCSISequence(unsigned int start, ctrlInfo &ci);

  /** Determin how many terminal rows are used by the current logical line.
   *
   *  This function computes the number of terminal rows occupied by
   *  the current logical line.  It takes any promptOffset into
   *  account.  The result is stored in lineRows.
   */
  void recalcLineRows(void);

  /** Goes one up or down in the history list updating list and display.
   *
   *  This function manages the history buffer of previous lines.  If
   *  @a dir is true, it moves one line up in the history, otherwise
   *  it moves down.  The current history entry @c histCur is replaced
   *  with the current line, and the previous or next history entry is
   *  loaded into current line and the display is updated.
   *
   *  If the top or bottom end of the history is reached, the terminal
   *  beeps.
   *
   *  @param dir : if true, move up (towards older entries) in the
   *  history, else move down.
   *
   *  @note Not implemneted.
   */
  void loadHistory(bool dir);


  /** Flag if terminal is in raw or cooked mode.  True if raw. */
  bool termIsRaw;

  
  /** Flag if terminal echo is on.
   *
   *  If true, then terminal echo is active and characters are printed
   *  as they are typed.  This means we need to take special action if
   *  isRaw is also true when control character (cursor movement,
   *  editing) are read, to redraw the line and resynchronize outr and
   *  the terminal idea of current cursor position.
   */
  bool termHasEchoOn;

  /** Number of columns in terminal.  If unknown, set to zero. */
  unsigned int termCols;

  /** Number of rows in terminal.  If unknown, set to zero. */
  unsigned int termRows;


  /** Current cursor position.
   *
   *  Current cursor position on the terminal.  This is in physical
   *  columns and rows, ignoring logical line longer then a physical
   *  terminal line.
   *
   *  Coordinates are 1-based, i.e. upper-left corner is (1;1)
   */
  unsigned int curTermCol;

  /** @copydoc curTermCol */
  unsigned int curTermRow;

  /** current character position in currently edited line.
   *
   *  This is the position in the @em logical line, it is @em not any
   *  column position on the terminal.
   *
   *  This is zero-based, i.e. 0 is left-most position.
   */
  unsigned int insertPoint;

  /** Current row/column coordinates in current edited line.
   *
   *  This is the current cursor position, relative to the starting
   *  row of the line, corresponding to insertPoint.  Note this is @em
   *  not a calculated by point2pos(), which is zero based.  To get
   *  curLineCol and curLineRow, add 1 to the result of point2pos().
   *
   *  This is 1-based, i.e. (1;1) is the left-most column in the
   *  terminal row where the line starts.
   */
  unsigned int curLineCol;

  /** @copydoc curLineCol 
   *
   *  This curLineRow + promptEndRow gives the 1-based terminal row
   *  number for the first row of the line.
   */
  unsigned int curLineRow;

  /** First column of editable text. 
   *
   *  This is the length of the last line of a prompt.  The physical
   *  terminal column @c promptOffset is the start position of
   *  editable text of a line composed with requestLine().
   *
   *  This is zero-based, i.e. zero means no offset, 1 means one
   *  character long prompt, making the line start at column 2 in the
   *  1-based terminal coordinate system.
   */
  unsigned int promptOffset;

  /** Terminal row where the line composed with requestLine()
   *  starts.
   *
   *  This is zero-based, top-most row has value zero.
   */
  unsigned int promptEndRow;

  /** Number of rows in the prompt text. */
  unsigned int promptRows;

  /** The prompt text, if any. 
   *
   *  Used only with requestLine().
   */
  std::string prompt;

  /** Saved cursor position for next print() call. */
  unsigned int printRow;

  /** Saved cursor position for next print() call. */
  unsigned int printCol;

  /** Number of terminal rows in current logical line. */
  unsigned int lineRows;

  /** Currently composed line. */
  std::string line;

  /** Last line completed and ready to be read by getLine(). */
  std::string savedLine;

  /** Storage space for characters killed (cut) with Ctrl-k from the
   *  input line.
   */
  std::string killBuffer;

  /** List of previous typed in lines.
   *
   *  This list (technically an array) hold the history of previously
   *  entered lines.  A line is added to the history, when it is read
   *  by getLine().  Dublicate consequtive entries are avoided if
   *  possible.
   */
  std::vector<std::string> historyList;

  /* input buffer management. */

  /** Next byte to be read from input buffer.
   *
   *  Index into inQueue, points to the first not yet processed byte
   *  in the buffer.
   */
  unsigned int queueNext;

  /** Number of vaild bytes in input buffer.
   *
   *  Number of not yet processed bytes in input buffer.  This
   *  queueLen + queueNext is the index into inQueue of the first free
   *  byte i.e where to store new bytes read from terminal.
   */
  unsigned int queueLen;

  /** Size of input buffer.
   *
   *  Total number of bytes in inQueue.
   */
  unsigned int queueSize;

  /** Input buffer.
   *
   *  This buffer holds the bytrs read from the terminal by
   *  readTerm(), until they are processed by composeLine().
   */
  char *inQueue;

  /** Handling of multi character sequences with getchar().
   *
   *  See comment in the implementation of getChar() and of update()
   *  for the details how this works.
   *
   *  If non-zero, index into inQueue where to store the next
   *  character read from the terminal.
   */
  unsigned int multibyteEnd;

  /** Handling of multi character sequences with getchar().
   *
   *  See comment in the implementation of getChar() and of update()
   *  for the details how this works.
   *
   *  If true, the last thing added to inQueue was a multi byte
   *  sequenc, otherwise it was a single character.
   */
  bool hasMultibyte;

  /** Default read timeout for calls to readTerm().
   *
   *  Initialized to 1 ms, can be set with setTimeout().
   */
  int termReadTimeout;  // must be signed, can be set to -1.

  /** Terminal state saved at Ctor call.
   *
   *  The Ctor saves the terminal state at class construction time
   *  into this field.  the saved state is used in the destructor to
   *  return the terminal to its previous state.
   */
  struct termios *savedTermState;

  /** Flag set by requestLine().
   *
   *  If true, then line requestLine() has been called, temporarly
   *  overriding the flags editMode, autoDiscard and forceEcho.
   */
  bool requestLineActive;

  /** Flag if recently a redrawLine(() occured.
   *
   *  This flag determines if we keep redrawing the line, in which
   *  case it will be completely erased, including prompt, the cursor
   *  reset to the start of the prompt, enough rows down cleared to
   *  hold the line and prompt and line redrawn.
   */
  bool redrawCalled;


#ifdef DEBUG_PSTERMIO
  /* Stuff related to debugging. */

public:
  /** @internal Write terminal state info to log file.
   *
   *  This function dumps the member variables related to terminal
   *  state to the log file PSTermIO::logfile.  The following
   *  variables are written out: termIsRaw, termHasEchoOn, termRows,
   *  termCols, curTermRow, curTermCol and termReadTimeout.
   *
   *  @param head Optinal header line to prepend to log data.
   */
  void logTermState(const char *head=0) ;

  /** @internal Write state info for line editing to log file.
   *
   *  This function dumps the member variables related to line editing
   *  to the log file PSTermIO::logfile.  The following variables are
   *  written out: insertPoint, lineRow, lineCol, lineRows,
   *  promptOffset, promptRow, curTermRow, curTermCol and the content
   *  of line.
   *
   *  @param head Optinal header line to prepend to log data.
   */
  void logEditState(const char *head=0);

  /** @internal Write state of input buffer to log file.
   *
   *  This function dumps the member variables related to the input
   *  buffer 'inQueue' to the log file PSTermIO::logfile.  The
   *  following variables are written out: queueNext, queueLen,
   *  hasMultibyte, multibyteEnd and the current content on inQueue.
   *
   *  @param head Optinal header line to prepend to log data.
   */
  void logInQueue(const char *head=0);

  /** Print text at @a buf to log file, showing non-printable
   *  characters in caret notation.
   *
   *  @param buf : text to print to log file.
   *
   *  @param endIx : number of characters in @a buf.
   */
  void dbgPrintStrCtrlCaret(char *buf, unsigned int endIdx);

  /** @internal Find sequenc of non-ctrl characters.
   *
   *  This function finds the longest sequence in buf, starting at
   *  index @a startIdx and at most extending up to endIdx, which is
   *  made entierely of printable characters.
   *
   *  @param buf : text buffer to search.
   *
   *  @param startIdx : where to start in @a buf, index of first valid
   *  character in @a buf.
   *
   *  @param endIdx : index of last valid character in buf.
   *
   *  @return length of sequence of printable character starting at @a
   *  startIdx.  If no such sequence exist (i.e. @a startIdy points to
   *  non-printable character), then returns zero.
   */
  unsigned int dbgNonCtrlSequence(const char *buf, unsigned int startIdx, unsigned int endIdx);

  /** Logfile for debug Info. */
  FILE *logfile;

private:
#endif

};


  /** Exception class used to report error conditions.
   *
   *  This class is derived from the C++ standard except
   *  "runtime_erroe" and is thrown by the PSTermIO function in case
   *  of an error condition.
   *
   *  This class is internal to PSTermIO and should not be
   *  instanciated by user vode.  Use it only to catch errors in your
   *  application in conjunction with a 'try .. catch()' block.
   */
  class PSTermIOException : public std::runtime_error {
  public:
    /** Default constructor. */
    explicit PSTermIOException();
    /** Copy constructor as required for C++ exception handling. */
    explicit PSTermIOException(PSTermIO::Error err);

    /** Custom constructor used by PSTermIO to construct an error
     *  report.
     *
     *  @param err Type of error as define in PSTermIO::Error enumeration.
     *
     *  @param txt Textual description of the error.
     *
     *  @param errorCode The libc errno value at the time the error
     *  occured.
     */
    explicit PSTermIOException(PSTermIO::Error err, const char *txt, int errorCode = 0);
    virtual ~PSTermIOException() throw();

  private:
    /** Translate error types to describing text string. */
    static const char *code2str(PSTermIO::Error err);

    /** The libc errno value at the time the error occured. */
    int lastErrno;
  };

} // namespace

#endif
/* End */
