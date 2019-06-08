#include <stdio.h>
#include <stdlib.h>
#include <iostream>
//#include <dlib/data_io.h>
#include <dlib/svm.h>
#include <dlib/random_forest.h>




using namespace std;
using namespace dlib;

typedef double raw_type; 
typedef matrix<raw_type,28,28> random_forest_data_type;
typedef double label_type;
typedef std::vector<random_forest_data_type> vec_type;



namespace dlib
{
    class mnist_dense_feature_extractor
    {

    public:
        typedef uint32_t feature;
        typedef matrix<double, 28, 28> sample_type;

        mnist_dense_feature_extractor(
        ) = default;

        void setup (
            const std::vector<sample_type>& x,
            const std::vector<double>& y 
        ) 
        {
            //DLIB_CASSERT(x.size() > 0);
            //DLIB_CASSERT(x.size() == y.size());
            for (auto& el : x)
                DLIB_CASSERT(el.size() == x[0].size(), "All the vectors in a training set have to have the same dimensionality.");

            DLIB_CASSERT(x[0].size() != 0, "The vectors can't be empty.");

            num_feats = x[0].size();
        }


        void get_random_features (
            dlib::rand& rnd,
            size_t num,
            std::vector<feature>& feats
        ) const
        {
            DLIB_ASSERT(max_num_feats() != 0);
            num = std::min(num, num_feats);

            feats.clear();
            for (size_t i = 0; i < num_feats; ++i)
                feats.push_back(i);

            // now pick num features at random
            for (size_t i = 0; i < num; ++i)
            {
                auto idx = rnd.get_integer_in_range(i,num_feats);
                std::swap(feats[i], feats[idx]);
            }
            feats.resize(num);
        }

        double extract_feature_value (
            const sample_type& item,
            const feature& f
        ) const
        {
            //DLIB_ASSERT(max_num_feats() != 0);

			int r = f%28;
			int c = f/28;
			

            return item(r,c);
        }

        size_t max_num_feats (
        ) const
        {
            return num_feats;
        }

        friend void serialize(const mnist_dense_feature_extractor& item, std::ostream& out)
        {
            serialize("mnist_dense_feature_extractor", out);
            serialize(item.num_feats, out);
        }

        friend void deserialize(mnist_dense_feature_extractor& item, std::istream& in)
        {
            check_serialized_version("mnist_dense_feature_extractor", in);
            deserialize(item.num_feats, in);
        }

    private:
        size_t num_feats = 0;
    };
}





/*
void random_forest_load_mnist_images( 
	std::string path, 
	std::vector<random_forest_data_type> &training_images, 
	std::vector<label_type> &training_labels, 
	std::vector<random_forest_data_type> &testing_images, 
	std::vector<label_type> &testing_labels )
{
*/
	
void random_forest_load_mnist_images( 
	std::string path, 
	std::string image_file,
	std::string label_file,
	std::vector<random_forest_data_type> &images, 
	std::vector<label_type> &labels, int count )
{
	
	unsigned char stash[28][28];

	FILE* fdata = fopen( ( path + "/" + image_file ).c_str(), "r" );
	FILE* flabels=fopen( ( path + "/" + label_file ).c_str(), "r");

	fread( stash, 1, 128/8, fdata );
	fread( stash, 1, 64/8, flabels );

	// Training data read	
	cout << "Loading data...." << endl;
	for( int c=0; c < count; c++ )
	{
		random_forest_data_type img;

		fread( stash, 28, 28, fdata );

		for( int i=0; i < 28; i++ )
		{
			for( int j=0; j < 28; j++ )
			{
				//img( i+(j*28), 0 ) = (raw_type) ( (stash[i][j]) );
				img( i, j ) = (raw_type) ( (stash[i][j]) );
				//img.push_back(  random_forest_data_type( ( (stash[i][j])  ) ) );
			}
		}
		images.push_back( img );


		fread( &stash[0][0], 1, 1, flabels );
		labels.push_back( (stash[0][0]) );
	}
	fclose( fdata );
	fclose( flabels );



	return;
}

 
void print_image( random_forest_data_type &img, unsigned long label, unsigned long predicted );
void print_random_forest_image( random_forest_data_type &img );

void print_probs( matrix<double,1,10> &softmax_probs, int x );

int main(int argc, char** argv) try
{

	bool correct_out = false;
	bool wrong_out = false;

    // This example is going to run on the MNIST dataset.  
    if (argc < 2)
    {
        cout << "This example needs the MNIST dataset to run!" << endl;
        cout << "You can get MNIST from http://yann.lecun.com/exdb/mnist/" << endl;
        cout << "Download the 4 files that comprise the dataset, decompress them, and" << endl;
        cout << "put them in a folder.  Then give that folder as input to this program." << endl;
        return 1;
    }


	if( argc > 3 )
	{
		int out_flag = atoi( argv[3] );

		if( out_flag == 1 )
			correct_out = true;
 
		if( out_flag == 2)
			wrong_out = true;

		if( out_flag == 3 )
			correct_out = wrong_out = true;
	}


    // MNIST is broken into two parts, a training set of 60000 images and a test set of
    // 10000 images.  Each image is labeled so that we know what hand written digit is
    // depicted.  These next statements load the dataset into memory.
    std::vector<random_forest_data_type> training_images;
    std::vector<label_type>         training_labels;
    std::vector<random_forest_data_type> testing_images;
    std::vector<label_type>         testing_labels;
    //load_mnist_dataset(argv[1], training_images, training_labels, testing_images, testing_labels);

    random_forest_load_mnist_images(argv[1], 
			"/train-images.idx3-ubyte", "/train-labels.idx1-ubyte", 
			training_images, training_labels, 60000 );

    random_forest_load_mnist_images(argv[1],
			"/t10k-images.idx3-ubyte", "/t10k-labels.idx1-ubyte", 
			testing_images, testing_labels, 10000 );


//===============================================================
    // The random_forest functions use column vectors to contain a lot of the data on which they
    // operate. So the first thing we do here is declare a convenient typedef.  

    // This typedef declares a matrix with 2 rows and 1 column.  It will be the object that
    // contains each of our 2 dimensional samples.   (Note that if you wanted more than 2
    // features in this vector you can simply change the 2 to something else.  Or if you
    // don't know how many features you want until runtime then you can put a 0 here and
    // use the matrix.set_size() member function)
//    typedef matrix<double, 2, 1> sample_type;
    typedef random_forest_data_type sample_type;

    // This is a typedef for the type of kernel we are going to use in this example.  In
    // this case I have selected the radial basis kernel that can operate on our 2D
    // sample_type objects
//    typedef radial_basis_kernel<sample_type> kernel_type;
  //  typedef radial_basis_kernel<random_forest_data_type> kernel_type;
	//typedef sparse_linear_kernel<random_forest_data_type> kernel_type;

//random_forest_multiclass_linear_trainer


    // Now we make objects to contain our samples and their respective labels.
//    std::vector<sample_type> samples;
//    std::vector<double> labels;





/*
    // Here we normalize all the samples by subtracting their mean and dividing by their
    // standard deviation.  This is generally a good idea since it often heads off
    // numerical stability problems and also prevents one large feature from smothering
    // others.  Doing this doesn't matter much in this example so I'm just doing this here
    // so you can see an easy way to accomplish this with the library.  
    // let the normalizer learn the mean and standard deviation of the samples

    // now normalize each sample
    vector_normalizer<random_forest_data_type> normalizer;


	if( !(argc > 2) )
	{
		cout << "normalizing training set" << endl;

		normalizer.train(training_images);

		for (unsigned long i = 0; i < training_images.size(); ++i)
		    training_images[i] = normalizer(training_images[i]); 

	    randomize_samples(training_images, training_labels);
	}


	{
		cout << "normalizing test set" << endl;

		normalizer.train(testing_images);

		for (unsigned long i = 0; i < testing_images.size(); ++i)
		    testing_images[i] = normalizer(testing_images[i]); 

    	randomize_samples(testing_images, testing_labels);
	}

    // Now that we have some data we want to train on it.  However, there are two
    // parameters to the training.  These are the nu and gamma parameters.  Our choice for
    // these parameters will influence how good the resulting decision function is.  To
    // test how good a particular choice of these parameters is we can use the
    // cross_validate_trainer() function to perform n-fold cross validation on our training
    // data.  However, there is a problem with the way we have sampled our distribution
    // above.  The problem is that there is a definite ordering to the samples.  That is,
    // the first half of the samples look like they are from a different distribution than
    // the second half.  This would screw up the cross validation process but we can fix it
    // by randomizing the order of the samples with the following function call.
    //randomize_samples(testing_images, testing_labels);


    // The nu parameter has a maximum value that is dependent on the ratio of the +1 to -1
    // labels in the training data.  This function finds that value.
//    const double max_nu = maximum_nu(training_labels);

    // here we make an instance of the random_forest_nu_trainer object that uses our kernel type.
//    random_forest_nu_trainer<kernel_type> random_forest_trainer;


	typedef linear_kernel<random_forest_data_type> kernel_type;

    typedef random_forest_multiclass_linear_trainer<kernel_type, label_type> trainer_type;

*/

    typedef random_forest_regression_trainer<mnist_dense_feature_extractor> trainer_type;

	trainer_type random_forest_trainer;
	random_forest_trainer.set_num_trees(100);
	random_forest_trainer.set_seed("random forest");

	std::vector<double> oobs;
	//trainer_type::trained_function_type df = random_forest_trainer.train(training_images, training_labels, oobs);
	trainer_type::trained_function_type df; // = random_forest_trainer.train(training_images, training_labels, oobs);


//	typedef one_vs_one_trainer<any_trainer<sample_type> > ovo_trainer;
//	ovo_trainer random_forest_trainer;




    // Next, we will make two different binary classification trainer objects.  One
    // which uses kernel ridge regression and RBF kernels and another which uses a
    // support vector machine and polynomial kernels.  The particular details don't matter.
    // The point of this part of the example is that you can use any kind of trainer object
    // with the one_vs_one_trainer.
//    typedef polynomial_kernel<random_forest_data_type> poly_kernel;
//    typedef radial_basis_kernel<random_forest_data_type> rbf_kernel;

    // make the binary trainers and set some parameters



    //random_forest_trainer.set_trainer(rbf_trainer);




    // Now let's do 5-fold cross-validation using the one_vs_one_trainer we just setup.
    // As an aside, always shuffle the order of the samples before doing cross validation.  
    // For a discussion of why this is a good idea see the random_forest_ex.cpp example.
//    randomize_samples(training_images, training_labels);
//    cout << "cross validation: \n" << cross_validate_multiclass_trainer(random_forest_trainer, training_images, training_labels, 5) << endl;




    // Next, if you wanted to obtain the decision rule learned by a one_vs_one_trainer you 
    // would store it into a one_vs_one_decision_function.

	
    //trainer_type::trained_function_type df;





	if( argc > 2 )
	{
		deserialize( argv[2] ) >> df;
	}
	else
	{
		cout << "training the random_forest" << endl;
		random_forest_trainer.be_verbose();
		df = random_forest_trainer.train(training_images, training_labels);
		serialize( "random_forest_mnist.dat" ) << df;
	}



	int num_right=0, num_wrong=0;

	for( int c = 0; c < testing_images.size(); c++ )
	{
		if( df(testing_images[c]) == testing_labels[c] )
		{
			num_right++;
		}
		else
		{
			num_wrong++;
			print_random_forest_image( testing_images[c] );
			cout 	<< "predicted label: "<< (int)df(testing_images[c])  
					<< ", true label: "<< (int)testing_labels[c] 
					<< endl;
		}

	}


    // The output is:
    /*
        predicted label: 2, true label: 2
        predicted label: 1, true label: 1
    */








    cout << "testing num_right: " << num_right << endl;
    cout << "testing num_wrong: " << num_wrong << endl;
    cout << "testing accuracy:  " << num_right/(double)(num_right+num_wrong) << endl;



}
catch(std::exception& e)
{
    cout << e.what() << endl;
}


void print_probs2( matrix<float,1,10> &softmax_probs, int x )
{
	for( int i=0; i < 10; i++ )
	{
		cout << i << "=" << softmax_probs(0,i) << "  |  ";
	}
	cout << endl;
}

void print_probs( matrix<float,1,10> &softmax_probs, int x )
{
	for( int i=0; i < 10; i++ )
	{
		printf( "%d\t", i );
	}
	cout << endl;

	for( int i=0; i < 10; i++ )
	{
		printf( "%0.1f%%\t", (float)softmax_probs(0,i) * 100.0 );
	}
	cout << endl;
}

void print_random_forest_image( random_forest_data_type &img )
{

	for( int i=0; i<28; i++ )
	{
		for( int j=0; j<28; j++ )
		{
			cout << ( (img( i, j ) > 150) ? '#' : 
						( (img( i, j ) > 75 ) ? '*' : 
							( (img( i, j ) > 50) ? '-' : ' ' ) ) );
		}
		cout << endl;
	}
	//cout << "label=" << label << "  predicted=" << predicted << endl;
}


