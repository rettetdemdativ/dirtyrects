Hi Michael,

As I mentioned, I’d like you to solve a small programming problem before we meet up. This isn’t an artificial exam question style thing, let alone a trick question. It’s a genuine problem that turned up last week in a project we’re working on, so hopefully representative of something you might encounter while working with us, but can still be explained and solved in isolation. The two of us came up with different solutions, which hopefully makes this a little more interesting too.


A few things I feel I should point out:

- Don’t feel like you have to spend a lot of time on this; you probably have better things to do. A working solution is of course much better than none, but we will certainly not be microbenchmarking anything. You’re also not competing with anyone. :-) If you only have time to solve one of the 2 parts, that’s also OK.

- If you come up with multiple possible solutions to either part, you don’t need to implement them all, but it would be interesting if you noted them down so we could talk about them.

- Please use C or C++ (whichever you prefer) for at least one solution. Using the language's standard libraries is fine, other libraries are almost certainly overkill.

- I’d appreciate if you didn’t hunt around the web for possible algorithms to solve it, at least not before you’ve come up with a solution yourself. Looking up standard library documentation etc., referring back to older code of yours, etc. are of course absolutely fine.

- I haven’t had a chance to try this question on anyone else, so if any part of it doesn’t make sense, or you feel there’s information missing, or my examples are wrong, don’t be afraid to ask.

- You’ll want to write some kind of test program around the solution(s). We’d definitely like to see that so we can run it as well as discussing. Something that will build without much trouble with gcc or clang on Linux or OS X would be good.


The situation:

We are working on a driver for a USB display device. The operating system periodically updates some memory containing the framebuffer with new image data, and also tells our driver about one or more rectangular areas that it has changed (“dirty rectangles"). We need to send at least the modified pixels to the device, but the device has 2 specific update modes which have different restrictions, and we have to support both. Given the dirty rectangles and the update restrictions, your code should produce a list of areas that should be sent to the device. (The 2 modes and solutions are independent, input data examples apply to both.)

Dirty rectangles are provided as:

struct rect
{
 uint32_t x1, y1, x2, y2;
};

x1, y1 are the coordinates for the the top left corner, inclusive, x2, y2 is the bottom right corner, exclusive. So x2-x1 is the width, y2-y1 is the height, the minimum values are 0 and the maximum values are the width/height of the whole display, respectively. You can assume that the maximum display size will be W=3840, H=2160. I’ll use the notation (x1,y1) - (x2, y2) in examples.

So you should have a function that accepts such a struct as an input:

void mark_dirty_rect(const struct rect* dirty);

This function will be called some number of times (1..N), and then there will be a call to a second function, get_update_rects(), which should return the list of update messages. You can add arguments to the functions to keep track of state between calls if necessary, or make the functions class members, etc. N is usually in the range 1-5, but there is technically no limit. Dirty rectangles might overlap, so some pixels might get marked more than once. You still only need to include them once in get_update_rects.


Part 1:

In mode 1, blocks with any height, but always the full width of the screen can be sent to the device. The solution will be the shortest possible sequence of non-overlapping ranges of y coordinates that covers all dirty rectangles. Update only pixel rows which need updating, but send as few ranges as possible.

For example, if the screen has W=1920, H=1080, and we have the following dirty rectangles:

(100, 50) - (250, 180)
(150, 150) - (1024, 200)
(256, 768) - (1664, 1024)

Then the optimal solution is to combine the first 2 (they overlap) and send the third separately, for a total of 2 messages:

start: 50, end: 200
and
start: 768, end: 1024

(start inclusive, end exclusive)

Implement mark_dirty_rect and get_update_rects to calculate the optimal ranges for any combination of dirty rectangles.



Part 2:

In mode 2, you can send updates to the device as rectangles which have both a width and a height that must be an integer multiple of 64. The smallest update is 64x64. Examples of allowed sizes are 192x128, 256x64, and 64x1024. 
The destination position’s x and y coordinates (top left corner) must each be multiples of 16, although you can use multiples of 64 if that makes it easier.

The solution will be a sequence of rectangles that cover all dirty rectangles and meet the above requirements.


Given the same example input as above:

(100, 50) - (250, 180)
(150, 150) - (1024, 200)
(256, 768) - (1664, 1024)

A possible solution is (using 64-pixel alignment, not 16):

(64, 0) - (256, 192)
(128, 192) - (256, 256)
(256, 128) - (1024, 256)
(256, 768) - (1664, 1024)

(The first 2 dirty rectangles overlap awkwardly, so we turn them into 3 messages, rounding to 64-pixel multiples. The third dirty rectangle is already correctly aligned.)


If the screen width or height is not a multiple of 64, you can send messages that overflow the screen edge up to the next multiple of 64. So for 1920x1080, you can send messages up to y2=1088 because 1080/64 = 16.875; rounding up to 17 gives 17 * 64 = 1088.


You don’t want to send too many individual messages or many pixels that don’t need updating, but it’s better to send slightly more messages and slightly more pixels than strictly necessary than to use an algorithm that will potentially take a long time to find a solution for complicated combinations. Never update the same pixel in more than one message.

Implement mark_dirty_rect and get_update_rects to calculate a suitable set of rectangles to send to the device.
(We did not use an algorithm that always finds an optimum solution, but which has very predictable performance.)