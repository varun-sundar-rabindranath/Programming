#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include "timer.h"
#include <assert.h>

using namespace wow_timer;

int main()
{
    char *outText;

    unsigned long long int lang_init_time;
    unsigned long long int text_detection_time;

    // Time the tesseract language init time 
    
    wow_timer::startTimer();
    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    // Initialize tesseract-ocr with English, without specifying tessdata path
    if (api->Init("/mnt/tesseract-test/tesseract-ocr/tessdata", "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }
    wow_timer::endTimer();
    lang_init_time = wow_timer::getElapsedTime();

    // Open input image with leptonica library
    Pix *image = pixRead("/mnt/tesseract-test/tesseract-ocr-test.png");
    api->SetImage(image);
    // Get OCR result
    wow_timer::startTimer();
    outText = api->GetUTF8Text();
    wow_timer::endTimer();
    text_detection_time = wow_timer::getElapsedTime();
    printf("OCR output:\n%s", outText);

    /*Count chars */
    int iter = 0;
    int char_count = 0;
    for(iter = 0; outText[iter] != '\0'; iter++) {
	char_count++;
    }

    printf("Found %d chars \n", char_count);
    printf("Tesseract init time = %llu microseconds \n", lang_init_time);
    printf("Tesseract Text detection time = %llu microseconds \n", text_detection_time);

    // Destroy used object and release memory
    api->End();
    delete [] outText;
    pixDestroy(&image);

    return 0;
}
