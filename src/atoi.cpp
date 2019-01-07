int atoi (int num, char* result) {
  int index = 0;
  if (num == 0) {
    result[index++] = '0';
    result[index++] = '\0';
    return 0;
  } 
  
  if (num < 0) {
    num *= -1;
    result[index++] = '-';
  }

  int size = 0;
  int tmp = num;
  while(tmp >0) {
    size++;
    tmp /= 10;
  }
  size++;
  char tmpStr[size];

  while (num > 0) {
    tmp = num % 10;
    tmpStr[index++] = '0' + tmp;
    num /= 10;
  }
  for (int i=0; i<size; i++) {
    result[i] = tmpStr[index-i-1];
  }
  result[index++] = '\0';
  return 0;
}
