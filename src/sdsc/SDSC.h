/*
 * SDSC.h
 *
 *  Created on: May 24, 2016
 *      Author: kenneth
 */

#ifndef SDSC_H_
#define SDSC_H_
#include "../skycube/Skycube.h"
#include "../utilities/config.h"
#include "../utilities/concurrent_queue.h"
#include "../utilities/gpu_struct.h"

template<int DIMS>
class SDSC : public Skycube<DIMS> {
public:
	SDSC(Config *cfg, int n);
	virtual ~SDSC();
	/**
	 * Initializes the data structures for the Skycube algorithm
	 * with the given input data.
	 * @param data An array of float arrays, with one array corresponding
	 * to each data point and each i'th element of an array specifiying the
	 * i'th attribute of the point.
	 * @post Initializes all the data structures so that Execute() can
	 * be invoked.
	 */
	void Init( float** data );

	/**
	 * Computes the skycube from the input dataset.
	 * @pre Assumes Init() has been invoked in order to initialize
	 * all the data structures.
	 * @post Populates the HashCube data structure with the skycube result.
	 */
	void Execute();

	lattice_node* getLattice();

	/**
	 * Retrieves the underlying data structure for the
	 * HashCube that was generated by a call to Execute().
	 * @pre Assumes Execute() has been invoked in order to populate the HashCube.
	 * @deprecated The access to the underlying data structure of the HashCube
	 * class has been deprecated. Use getHashCube() instead.
	 */
	std::map<unsigned int, std::vector<int> >* getHashCubeStructure();

	/**
	 * Retrieves the HashCube that was generated by a call to Execute().
	 * @pre Assumes Execute() has been invoked in order to populate the HashCube.
	 */
	const HashCube< DIMS >& getHashCube();

	/** A bitmask wherein bits are set only for the active dimensions */
	const static uint32_t active_bits_ = ( ( 1 << DIMS ) -1 );

private:

	uint32_t n_; /**< The number of data points to be processed. */
	uint32_t accum_;
	uint32_t pq_size_;
	int max_d_;
	int full_index_;

	lattice_node* lattice_;
	HybridTuple<DIMS> *data_;
	float *gpu_data;
	HashCube< DIMS > hashcube_;
	Config *cfg;
	concurrent_queue<int> task_queue;
	std::vector<SDSCGpu*> devices;

	inline void initTuple(uint32_t index, HybridTuple<DIMS> *next);
	inline int findMinParent(int next);
	inline void initializeGpu(SDSCGpu *conf);
	inline void freeGpu(SDSCGpu *dev);
	void gpu_processing_thread(SDSCGpu *conf);
	void cpu_processing_thread(int threads);
};

#endif /* SDSC_H_ */
