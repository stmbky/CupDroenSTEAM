

#ifndef _FIFOARRAY_H
#define _FIFOARRAY_H

class FIFOArray
{
    private:
        int bufSize;
        int head;
        int tail;
        uint8_t *buf;
        
    public:
        FIFOArray(int size){
			buf =  malloc (size);
			bufSize = size;
		}
        ~FIFOArray(){
			free(buf);
		}
        void push (uint8_t ch){
			int h = ((head+1)>=bufSize)? 0 : head+1;

			if(h == tail)
				return;

			buf[head++] = ch;
			if(head >= bufSize)
				head = 0;
		}
        uint8_t pop(void){
			uint8_t ch;

			if(tail == head)
				return 0;

			ch = buf[tail];
			if(++tail >= bufSize)
				tail = 0;
			return ch;
		}
        uint8_t peek(int offset){
			offset += tail;
			if(offset >= bufSize)
				offset -= bufSize;

			return buf[offset];
		}
        uint8_t count(void){
			return (head >= tail)? head-tail : head+bufSize-tail;
		}
        void clearArray(int len){
			for(int k=0; k<len; k++)
				pop();
		}
};

#endif
