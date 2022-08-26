# death_notice_override
Overrides the formatting function used in the gathering for death notice icons and changes the extension from .svg to .png

## Why?
Changing the images from being gathered using .png instead of .svg cannot be done thru css or xml from what I tested since the image is gathered inside of the csgo code
So I just reversed the function that formatted the string and I changed the extension to .png lol

## How to use this
Dump the images from the old .swf files from older csgo versions (white ones), rename them to their corresponding .svg counterparts, put inside of ur migi addons and rebuild
After you inject it should be using all the .png images you dumped and put inside your migi addon
