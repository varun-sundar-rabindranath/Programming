<?php
    $files = glob("/home/varun/programming/OpenCV/adaptive*");
    echo basename($files[0]) . "\n";
    echo $files[1] . "\n";
    echo $files[2] . "\n";

    //foreach (glob("/home/varun/programming/OpenCV/*.cpp") as $filename) {
    //    echo "$filename size " . filesize($filename) . "\n";
    //}
?>
