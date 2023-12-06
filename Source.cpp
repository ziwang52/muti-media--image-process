
#include <iostream>
#include <fstream>
#include <string>
#include<random>
#include<cfloat>
#include<vector>

#define RGB_COMPONENT_COLOR 255

using namespace std;

typedef struct {
	unsigned char red, green, blue;
} PPMPixel;

typedef struct {
	int x, y;
	PPMPixel* data;
} PPMImage;



void medain_filter(PPMImage* image)
{
int i,j;
	//declare variables  
	int* red = new int[8];
	int* green = new int[8];
	int* blue = new int[8];

	int* pexiel_store_r = new int[8];
	int* pexiel_store_g = new int[8];
	int* pexiel_store_b = new int[8];

int counter_blue = 0;
int counter_red = 0;
int counter_green = 0;
 	int a = image->x;
	int b = image->y;
	//new temp array to store
	int* image3 = new int[a * b];
	int* image4 = new int[a * b];
	int* image5 = new int[a * b];

	for (i = 0; i < a * b; i++)
	{

		for (j = 0; j < b; j++)
		{

		}
		image3[i] = image->data[i].red;
		image4[i] = image->data[i].green;
		image5[i] = image->data[i].blue;


	}

	//store and calculate pixel values	
	for (i = 1; i < (a-1 ); i++)
	{
		for (j = 1; j < (b-1  ); j++)
		{
			red[0] = image3[(i - 1) + a * (j - 1)]; //top left
			green[0] = image4[(i - 1) + a * (j - 1)]; //top left
			blue[0] = image5[(i - 1) + a * (j - 1)]; //top left

			red[1] = image3[(i - 1) + a * j]; //top center
			green[1] = image4[(i - 1) + a * j]; //top center
			blue[1] = image5[(i - 1) + a * j]; //top center

			red[2] = image3[(i - 1) + a * (j + 1)]; //top right
			green[2] = image4[(i - 1) + a * (j + 1)]; //top right
			blue[2] = image5[(i - 1) + a * (j + 1)]; //top right

			red[3] = image3[(i)+a * (j - 1)]; // left
			green[3] = image4[(i)+a * (j - 1)]; // left
			blue[3] = image5[(i)+a * (j - 1)]; // left

			red[4] = image3[(i)+a * (j + 1)]; // right
			green[4] = image4[(i)+a * (j + 1)]; // right
			blue[4] = image5[(i)+a * (j + 1)]; // right


			red[5] = image3[(i + 1) + a * (j - 1)]; //bottom left
			green[5] = image4[(i + 1) + a * (j - 1)]; //bottom left
			blue[5] = image5[(i + 1) + a * (j - 1)]; //bottom left

			red[6] = image3[(i + 1) + a * (j)]; // bottom center
			green[6] = image4[(i + 1) + a * (j)]; // bottom center
			blue[6] = image5[(i + 1) + a * (j)]; // bottom center


			red[7] = image3[(i + 1) + a * (j + 1)]; // bottom right
			green[7] = image4[(i + 1) + a * (j + 1)]; // bottom right
			blue[7] = image5[(i + 1) + a * (j + 1)]; // bottom right

			//store filter value 
			/*sorting*/
			for (int sort = 0; sort < 8; sort++)
			{
				//                x=window[sort];
				for (int y = 0; y < 8; y++)
				{
					if ((red[sort] > red[y])/*&&(y>sort)*/)
					{
						counter_red++;
					}
					if ((green[sort] > green[y])/*&&(y>sort)*/)
					{
						counter_green++;
					}
					if ((blue[sort] > blue[y])/*&&(y>sort)*/)
					{
						counter_blue++;

					}
			

				}
		
				pexiel_store_r[counter_red] = red[sort];
				pexiel_store_g[counter_green] = green[sort];
				pexiel_store_b[counter_blue] = blue[sort];

				counter_blue = 0;
				counter_red = 0;
				counter_green = 0;
			}




			//change to original photo 
			image->data[i + a * j].red = (pexiel_store_r[4] + pexiel_store_r[3]) / 2;
			image->data[i + a * j].green = (pexiel_store_g[4] + pexiel_store_g[3]) / 2;
			image->data[i + a * j].blue = (pexiel_store_b[4] + pexiel_store_b[3]) / 2;
		}
	}
}
 

static PPMImage* readPPM(const char* filename)
{
	char buff[16];
	PPMImage* img;
	FILE* fp;
	int c, rgb_comp_color;
	//open PPM file for reading
	fp = fopen(filename, "rb");//reading a binary file
	if (!fp) {
		fprintf(stderr, "Unable to open file '%s'\n", filename);
		exit(1);
	}

	//read image format
	if (!fgets(buff, sizeof(buff), fp)) {
		perror(filename);
		exit(1);
	}

	//check the image format can be P3 or P6. P3:data is in ASCII format P6: data is in byte format
	if (buff[0] != 'P' || buff[1] != '6') {
		fprintf(stderr, "Invalid image format (must be 'P6')\n");
		exit(1);
	}

	//alloc memory form image
	img = (PPMImage*)malloc(sizeof(PPMImage));
	if (!img) {
		fprintf(stderr, "Unable to allocate memory\n");
		exit(1);
	}

	//check for comments
	c = getc(fp);
	while (c == '#') {
		while (getc(fp) != '\n');
		c = getc(fp);
	}

	ungetc(c, fp);//last character read was out back
	//read image size information
	if (fscanf(fp, "%d %d", &img->x, &img->y) != 2) {//if not reading widht and height of image means if there is no 2 values
		fprintf(stderr, "Invalid image size (error loading '%s')\n", filename);
		exit(1);
	}

	//read rgb component
	if (fscanf(fp, "%d", &rgb_comp_color) != 1) {
		fprintf(stderr, "Invalid rgb component (error loading '%s')\n", filename);
		exit(1);
	}

	//check rgb component depth
	if (rgb_comp_color != RGB_COMPONENT_COLOR) {
		fprintf(stderr, "'%s' does not have 8-bits components\n", filename);
		exit(1);
	}

	while (fgetc(fp) != '\n');
	//memory allocation for pixel data for all pixels
	img->data = (PPMPixel*)malloc(img->x * img->y * sizeof(PPMPixel));

	if (!img) {
		fprintf(stderr, "Unable to allocate memory\n");
		exit(1);
	}

	//read pixel data from file
	if (fread(img->data, 3 * img->x, img->y, fp) != img->y) { //3 channels, RGB  //size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
		/*ptr − This is the pointer to a block of memory with a minimum size of size*nmemb bytes.
			size − This is the size in bytes of each element to be read.
			nmemb − This is the number of elements, each one with a size of size bytes.
			stream − This is the pointer to a FILE object that specifies an input stream.
		*/
		fprintf(stderr, "Error loading image '%s'\n", filename);
		exit(1);
	}

	fclose(fp);
	return img;
}


void writePPM(const char* filename, PPMImage* img)
{
	FILE* fp;
	//open file for output
	fp = fopen(filename, "wb");//writing in binary format
	if (!fp) {
		fprintf(stderr, "Unable to open file '%s'\n", filename);
		exit(1);
	}

	//write the header file
	//image format
	fprintf(fp, "P6\n");

	//image size
	fprintf(fp, "%d %d\n", img->x, img->y);

	// rgb component depth
	fprintf(fp, "%d\n", RGB_COMPONENT_COLOR);

	// pixel data
	fwrite(img->data, 3 * img->x, img->y, fp);
	fclose(fp);
}

void changeColorPPM(PPMImage* img)//Negating image
{
	int i;
	if (img) {

		for (i = 0; i < img->x * img->y; i++) {
			int avg = (img->data[i].red + img->data[i].green + img->data[i].blue) / 3;
			img->data[i].red = avg;
			img->data[i].green = avg;
			img->data[i].blue = avg;
		}
	}
}

void addNoise(PPMImage* image)
{
	int num_point = image->x * image->y;
	//SET noise percentage 
 double percent = 0.05;
 int num_noise = 0;
	int* label = new int[num_point];
 
	//initialization

	//get first center via using redaon selection
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> select(0, num_point - 1);

	random_device rn;
	mt19937 rand(rn());
//modify noise pixel value range 
	uniform_int_distribution<> pick(0, 255);

	num_noise = num_point* percent;

	for (int i = 0; i < num_noise; i++)
	{
		int rand_points = select(gen);
		int rand_noise = pick(rand);

	 image->data[rand_points].red= rand_noise;
		image->data[rand_points].green= rand_noise;
	 image->data[rand_points].blue  = rand_noise;
	}

}
void addNoise_Gaussian(PPMImage* image)
{
	int num_point = image->x * image->y;

//SET noise percentage 
 double percent = 0.05;
 int num_noise = 0;
	int* label = new int[num_point];
 
	//initialization

	//get first center via using redaon selection
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> select(0, num_point - 1);

	random_device rn;
	mt19937 rand(rn());
	uniform_int_distribution<> pick(0, 270);

	num_noise = num_point* percent;

	for (int i = 0; i < num_noise; i++)
	{
		int rand_points = select(gen);
		int rand_noise = pick(rand);

	 image->data[rand_points].red= rand_noise;
		image->data[rand_points].green= rand_noise;
	 image->data[rand_points].blue  = rand_noise;
	}

}
void sobel_serial(PPMImage* image)
{
	//declare variables  

	int gx, gy, x1, y1, i, j, p1, p2, p3, p4, p5, p6, p7, p8, p9;
	int a = image->x;
	int b = image->y;
	//new temp array to store
	int* image3 = new int[a * b];
	for (i = 1; i < a * b; i++) {
		image3[i] = image->data[i].red;
	}

	//store and calculate pixel values	
	for (i = 1; i < (a - 1); i++)
	{
		for (j = 1; j < (b - 1); j++)
		{

			p1 = image3[(i - 1) + a * (j - 1)];
			p2 = image3[(i - 1) + a * j];
			p3 = image3[(i - 1) + a * (j + 1)];
			p4 = image3[(i)+a * (j - 1)];
			p5 = image3[(i)+a * (j)];
			p6 = image3[(i)+a * (j + 1)];
			p7 = image3[(i + 1) + a * (j - 1)];
			p8 = image3[(i + 1) + a * (j)];
			p9 = image3[(i + 1) + a * (j + 1)];

			//store filter value 
			gx = p1 + 2 * p2 + p3 - p7 - 2 * p8 - p9;
			gy = p3 + 2 * p6 + p9 - p1 - 2 * p4 - p7;
			//change to original photo 
			image->data[i + a * j].red = sqrt(gx * gx + gy * gy);
			image->data[i + a * j].green = sqrt(gx * gx + gy * gy);
			image->data[i + a * j].blue = sqrt(gx * gx + gy * gy);
		}
	}

}

int main()
{
	PPMImage* image;
	PPMImage* image_noise;
 


 
	//	image = readPPM("sample.ppm");
	image = readPPM("model1.ppm");
	image_noise = readPPM("model1.ppm");


	int x = image->x;
	int y = image->y;
	int i;
//edge deduction original
	changeColorPPM(image);
	//declare new array to store photo
	int* image1 = new int[x * y];


	for (i = 1; i < x * y; i++) {
		image1[i] = image->data[i].red;

	}

	sobel_serial(image);
	cout << "process edge deduction on original image......" << endl;
	writePPM("edge_deduction_original.ppm", image);


	addNoise(image_noise);
	cout << "add salt&papper noise into original image......" << endl;
	writePPM("salt&papper.ppm", image_noise);
	// declare new array to store photo
 

	medain_filter(image_noise);
	cout << " processin gmedain_filter......" << endl;
	//writePPM("medain_filter-use original .ppm", image);

	writePPM("after_medain_filter.ppm", image_noise);


// edge deduction after remove noise
  
	changeColorPPM(image_noise);
	//declare new array to store photo
		int* image2 = new int[x * y];


	for (i = 1; i < x * y; i++) {
		image2[i] = image_noise->data[i].red;

	}

	sobel_serial(image_noise);

 // kmean after remove noise 

	PPMImage* image3;

	//	image = readPPM("sample.ppm");
	//image3 = readPPM("after_medain_filter.ppm");
	image3 = readPPM("model1.ppm");

	//image3 = readPPM("model_noise.ppm");


 // cluster number setting 
	int k = 10;

	int num_point = x * y;



	double new_sse = 0.0;
	double sse = 1.0;


	int* label = new int[num_point];
	for (i = 1; i < num_point; i++) {
		label[i] = 0;
	}
	int* center_r = new int[k];
	int* center_g = new int[k];
	int* center_b = new int[k];

	int* new_center_r = new int[k];
	int* new_center_g = new int[k];
	int* new_center_b = new int[k];

	for (i = 0; i < k; i++) {

		center_r[i] = 0;
		center_g[i] = 0;
		center_b[i] = 0;
		new_center_r[i] = 0;
		new_center_g[i] = 0;
		new_center_b[i] = 0;
	}



	//get first center
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> select(0, num_point - 1);
	for (int i = 0; i < k; i++)
	{
		int rand_points = select(gen);


		center_r[i] = image3->data[rand_points].red;
		center_g[i] = image3->data[rand_points].green;
		center_b[i] = image3->data[rand_points].blue;

	}

	int index = 0;





	while (abs(new_sse - sse) >= 0.2)
	{
		new_sse = sse;


		if (index != 0)
		{
			for (int i = 0; i < k; i++)
			{

				center_r[i] = new_center_r[i];
				center_g[i] = new_center_g[i];
				center_b[i] = new_center_b[i];
			}
		}


		// label each point in the pic 


//calaulate dis
		for (i = 1; i < x * y; i++)

		{
			double min_dis = DBL_MAX;
			for (int j = 0; j < k; j++)
			{
				//double distance = sqrt(pow((image->data[i].red - center_r[j]), 2) + pow((image->data[i].green - center_g[j]), 2) + pow((image->data[i].blue - center_b[j]), 2));
				double distance = ((image3->data[i].red - center_r[j]) * (image3->data[i].red - center_r[j]) + ((image3->data[i].green - center_g[j]) * (image3->data[i].green - center_g[j])) + ((image3->data[i].blue - center_b[j]) * (image3->data[i].blue - center_b[j])));

				{
					if (distance < min_dis)
					{
						min_dis = distance;
						label[i] = j;
					}
				}

			}

		}


		//get new center 
		int* count = new int[num_point];
		int* r = new int[num_point];
		int* g = new int[num_point];
		int* b = new int[num_point];
		for (int i = 1; i < num_point; i++)
		{

			b[i] = 0;
			g[i] = 0;
			r[i] = 0;
			count[i] = 0;

		}
		for (int i = 1; i < num_point; i++)
		{
			for (int j = 0; j < k; j++)
			{
				// group each cluster
				if (j == label[i])
				{
					count[j]++;
					b[j] += image3->data[i].blue;
					g[j] += image3->data[i].green;
					r[j] += image3->data[i].red;

				}
			}

		}

		for (int i = 0; i < k; i++)
		{

			b[i] /= count[i];
			g[i] /= count[i];
			r[i] /= count[i];

		}


		for (int i = 1; i < num_point; i++)
		{
			for (int j = 0; j < k; j++)
			{
				// group each cluster
				if (j == label[i])
				{
					new_center_r[j] = r[j];
					new_center_g[j] = g[j];
					new_center_b[j] = b[j];

				}
			}

		}


		delete[] count;
		delete[] r;
		delete[] g;
		delete[] b;

		sse = 0.0;

		for (int j = 0; j < k; j++)
		{


			sse += (((new_center_r[j] - center_r[j]) * (new_center_r[j] - center_r[j])) + ((new_center_g[j] - center_g[j]) * (new_center_g[j] - center_g[j])) + ((new_center_b[j] - center_b[j]) * (new_center_b[j] - center_b[j])));

		}




		index++;
		cout << " iteration: " << index << "  SSE:  " << sse << endl;
	}

	for (int i = 1; i < num_point; i++)
	{
		image3->data[i].red = center_r[label[i]];
		image3->data[i].green = center_g[label[i]];
		image3->data[i].blue = center_b[label[i]];
	}
	cout << "itreation use:" << index << endl;
	//writePPM("k_mean_output-befroe remove noise.ppm", image3);
		//writePPM("k_mean_output-after remove noise.ppm", image3);

	writePPM("k_mean_output-after remove noise 20.ppm", image3);


	//edge_ deduction use K-means output image

	cout << " processing k_mean_output - after remove noise......" << endl;



	
	
	
	
	
	
	printf("Press any key...");
	getchar();





 
}