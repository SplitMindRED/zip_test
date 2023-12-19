#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "zlib.h"
#include <iostream>

#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/highgui.hpp>

using std::cout;
using std::endl;

void test_zip()
{
  const char* src_data = "some foo";
  ulong src_len = strlen(src_data) + 1;          // +1 for the trailing `\0`
  ulong compressed_len = compressBound(src_len); // this is how you should estimate size
                                                 // needed for the buffer

  cout << "src len: " << (int)src_len << endl;
  cout << "compressed est len: " << (int)compressed_len << endl;

  char* compressed_data = (char*)malloc(compressed_len);
  int res = compress((Bytef*)compressed_data, &compressed_len, (const Bytef*)src_data, src_len);

  cout << "compressed len: " << (int)compressed_len << endl;

  if (res == Z_BUF_ERROR)
  {
    printf("Buffer was too small!\n");

    return;
  }

  if (res == Z_MEM_ERROR)
  {
    printf("Not enough memory for compression!\n");

    return;
  }

  // destLen is now the size of actuall buffer needed for compression
  // you don't want to uncompress whole buffer later, just the used part

  char* uncompressed_data = (char*)malloc(src_len);
  int des = uncompress((Bytef*)uncompressed_data, &src_len, (const Bytef*)compressed_data, compressed_len);
  cout << "After uncompressing: " << uncompressed_data << endl;
}

int main()
{
  cv::Mat img = cv::imread("../tmp.png");
  cv::Mat img2 = cv::imread("../tmp.png");

  // cv::imshow("tmp", img);
  // cv::waitKey(0);

  const uchar* src_data = img.data;
  ulong src_len = img.total() * 3;               // +1 for the trailing `\0`
  ulong compressed_len = compressBound(src_len); // this is how you should estimate size
                                                 // needed for the buffer

  cout << "src len: " << (int)src_len << endl;
  cout << "compressed est len: " << (int)compressed_len << endl;

  uchar* compressed_data = (uchar*)malloc(compressed_len);

  auto start = std::chrono::system_clock::now();
  // int res = compress((Bytef*)compressed_data, &compressed_len, (const Bytef*)src_data, src_len);
  int res = compress2((Bytef*)compressed_data, &compressed_len, (const Bytef*)src_data, src_len, Z_BEST_SPEED);
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> compress_time = end - start;

  cout << "compress time: " << compress_time.count() << endl;
  cout << "compressed est len: " << (int)compressed_len << endl;

  if (res == Z_BUF_ERROR)
  {
    printf("Buffer was too small!\n");

    return 1;
  }

  if (res == Z_MEM_ERROR)
  {
    printf("Not enough memory for compression!\n");

    return 2;
  }

  // destLen is now the size of actuall buffer needed for compression
  // you don't want to uncompress whole buffer later, just the used part

  uchar* uncompressed_data = (uchar*)malloc(src_len);
  int des = uncompress((Bytef*)uncompressed_data, &src_len, (const Bytef*)compressed_data, compressed_len);
  // cout << "After uncompressing: " << uncompressed_data << endl;

  memcpy(img2.data, uncompressed_data, src_len);

  // cv::imshow("tmp2", img2);
  // cv::imshow("tmp delta", img2 - img);
  // cv::waitKey(0);

  return 0;
}