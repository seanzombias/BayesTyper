
/*
VariantClusterHaplotypes.hpp - This file is part of BayesTyper (https://github.com/bioinformatics-centre/BayesTyper)


The MIT License (MIT)

Copyright (c) 2016 Jonas Andreas Sibbesen and Lasse Maretty

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/


#ifndef __bayesTyper__VariantClusterHaplotypes_hpp
#define __bayesTyper__VariantClusterHaplotypes_hpp

#include <vector>
#include <unordered_map>
#include <random>

#include "boost/functional/hash.hpp"

#include "Utils.hpp"
#include "KmerCounts.hpp"
#include "KmerStats.hpp"
#include "Sample.hpp"
#include "VariantInfo.hpp"


class VariantClusterHaplotypes {

	public:

		VariantClusterHaplotypes() {};

		struct HaplotypeInfo {

			vector<ushort> variant_allele_indices;
			vector<uint> nested_variant_cluster_indices;

			HaplotypeInfo(const ushort num_variants) : variant_allele_indices(num_variants, Utils::ushort_overflow) {}
		};

		vector<HaplotypeInfo> haplotypes;

		Utils::MatrixXuchar haplotype_kmer_multiplicities;		

		struct KmerInfo {

			KmerCounts * const counts;
			const uchar bias_idx;
			vector<pair<ushort, vector<bool> > > variant_haplotype_indices;

			KmerInfo(KmerCounts * const counts_in, const uchar bias_idx_in) : counts(counts_in), bias_idx(bias_idx_in) {}
		};

		vector<KmerInfo> kmers;

		vector<uint> unique_kmer_indices;
		vector<uint> multicluster_kmer_indices;

		vector<uint> unique_kmer_subset_indices;
		vector<uint> multicluster_kmer_subset_indices;

		Utils::MatrixXuchar sample_multicluster_kmer_multiplicities;

		struct KmerStatsCache {

			bool update;
			
			vector<KmerStats> haplotype_1;
			vector<KmerStats> haplotype_2;

			KmerStatsCache(const ushort num_variants) {

				update = true;

				haplotype_1 = vector<KmerStats>(num_variants);
				haplotype_2 = vector<KmerStats>(num_variants);
			}
		};

		vector<KmerStatsCache> kmer_stats_cache;

		unordered_map<uint, vector<ushort> > nested_variant_cluster_dependency;

		struct NestedVariantClusterInfo {

			Utils::Ploidy nested_ploidy;
			vector<KmerStats> nested_kmer_stats;

			NestedVariantClusterInfo(const Utils::Ploidy chrom_ploidy_in) : nested_ploidy(chrom_ploidy_in) {}
		};

		uchar getUniqueKmerMultiplicity(const uint, const pair<ushort,ushort> &, const Utils::Gender);
		uchar getMulticlusterKmerMultiplicity(const uint, const pair<ushort,ushort> &, const pair<ushort,ushort> &, const ushort, const Utils::Gender);
		uchar getPreviousMulticlusterKmerMultiplicity(const uint, const pair<ushort,ushort> &, const pair<ushort,ushort> &, const ushort, const Utils::Gender);

		void sampleKmerSubset(mt19937 *, const float, const uint, const ushort);

		bool isMulticlusterKmerUpdated(const uint, const ushort);
		void updateMulticlusterKmerMultiplicities(const pair<ushort,ushort> &, const pair<ushort,ushort> &, const ushort);
		
		void updateAlleleKmerStats(vector<vector<AlleleKmerStats> > *, const vector<Sample> &, const vector<VariantInfo> &, const vector<NestedVariantClusterInfo> &, const vector<pair<ushort,ushort> > &); 

	private:

		uchar getDiplotypeKmerMultiplicity(const uint, const pair<ushort,ushort> &);

		bool isMaxHaplotypeVariantKmer(vector<vector<uint> > *, const uint, const vector<pair<ushort, vector<bool> > > &);

		void updateKmerStatsCache(KmerInfo &, const pair<ushort,ushort> &, const ushort, const uchar);
		void addHaplotypeKmerStats(vector<vector<AlleleKmerStats> > *, const vector<KmerStats> &, const vector<VariantInfo> &, const ushort, const vector<ushort> &);
		void addNestedHaplotypeKmerStats(vector<vector<AlleleKmerStats> > *, const KmerStats &, const vector<VariantInfo> &, const ushort);
};


#endif