#include <Pixy2.h>
 // This is the main Pixy object
 Pixy2 pixy;
 void setup()
{
 Serial.begin(115200);
 Serial.print("Starting... \n");
 pixy.init();
}
 void loop()
{
 int i;
// grab blocks!
pixy.ccc.getblocks();
//If there are detect blocks,print them!
if (pixy.ccc,numBlocks)
{
Serial.print("Detected ");
Serial.print(pixy .ccc.numBlocks);
for (i=0; i<pixy.ccc.numBlocks; i++)
{
Serial.print(" block");
Serial.print(i);
Serial.print(": ");
pixy.ccc.blocks[i].print();
}
}
}
