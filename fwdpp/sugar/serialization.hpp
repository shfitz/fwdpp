#ifndef __FWDPP_SUGAR_SINGLEPOP_SERIALIZATON_HPP__
#define __FWDPP_SUGAR_SINGLEPOP_SERIALIZATON_HPP__

#include <iosfwd>
#include <sstream>
#include <algorithm>
#include <type_traits>
#include <fwdpp/IO.hpp>
#include <fwdpp/sugar/poptypes/tags.hpp>
#include <fwdpp/sugar/popgenmut.hpp>
#include <iostream>
namespace KTfwd
{
  struct mutation_writer
  {
    using result_type = void;
    template<typename mutation_t>
    inline typename std::enable_if<std::is_same<mutation_t,popgenmut>::value,result_type>::type
    operator()( const mutation_t &m,
		std::ostream & buffer) const
    {
      buffer.write( reinterpret_cast<const char *>(&m.n),sizeof(unsigned));
      buffer.write( reinterpret_cast<const char *>(&m.g),sizeof(unsigned));
      buffer.write( reinterpret_cast<const char *>(&m.pos),sizeof(double));
      buffer.write( reinterpret_cast<const char *>(&m.s),sizeof(double));
      buffer.write( reinterpret_cast<const char *>(&m.h),sizeof(double));
    }
    template<typename mutation_t>
    inline typename std::enable_if<std::is_same<mutation_t,mutation>::value,result_type>::type
    operator()( const mutation_t &m,
		std::ostream & buffer) const
    {
      buffer.write( reinterpret_cast<const char *>(&m.n),sizeof(unsigned));
      buffer.write( reinterpret_cast<const char *>(&m.pos),sizeof(double));
      buffer.write( reinterpret_cast<const char *>(&m.s),sizeof(double));
      buffer.write( reinterpret_cast<const char *>(&m.h),sizeof(double));
    }
  };

  template<typename mutation_t>
  struct mutation_reader
  {
    using result_type = mutation_t;
    template<typename U = mutation_t>
    inline typename std::enable_if<std::is_same<U,popgenmut>::value,result_type>::type
    operator()( std::istream & in ) const
    {
      unsigned n,g;
      bool neutral;
      double pos,s,h;
      in.read( reinterpret_cast<char *>(&n),sizeof(unsigned));
      in.read( reinterpret_cast<char *>(&neutral),sizeof(bool));
      in.read( reinterpret_cast<char *>(&g),sizeof(unsigned));
      in.read( reinterpret_cast<char *>(&pos),sizeof(double));
      in.read( reinterpret_cast<char *>(&s),sizeof(double));
      in.read( reinterpret_cast<char *>(&h),sizeof(double));
      return result_type(pos,s,h,g,n);
    }
    template<typename U = mutation_t>
    inline typename std::enable_if<std::is_same<U,mutation>::value,result_type>::type
    operator()( std::istream & in ) const
    {
      unsigned n,g;
      bool neutral;
      double pos,s,h;
      in.read( reinterpret_cast<char *>(&n),sizeof(unsigned));
      in.read( reinterpret_cast<char *>(&pos),sizeof(double));
      in.read( reinterpret_cast<char *>(&s),sizeof(double));
      in.read( reinterpret_cast<char *>(&h),sizeof(double));
      return result_type(pos,s,n,h);
    }
  };
  
  class serialize
  {
  public:
    using result_type = void;
  private:
    template<typename sugarpop_t,
	     typename writer_t>
    result_type do_work( const sugarpop_t & pop,
			 const writer_t & wt ) const
    {
      write_binary_pop(&pop.gametes,&pop.mutations,&pop.diploids,wt,buffer);
      //Step 2: output fixations 
      unsigned temp = pop.fixations.size();
      buffer.write( reinterpret_cast<char*>(&temp), sizeof(unsigned) );
      std::for_each( pop.fixations.begin(), pop.fixations.end(),
		     std::bind(std::cref(wt),std::placeholders::_1,std::ref(buffer)) );
      //Step 3:the fixation times
      buffer.write( reinterpret_cast<const char *>(&pop.fixation_times[0]), pop.fixation_times.size()*sizeof(unsigned) );
    }
    template<typename sugarpop_t,
	     typename writer_t>
    result_type do_work_metapop( const sugarpop_t & pop,
				 const writer_t & wt ) const
    {
      write_binary_metapop(&pop.gametes,&pop.mutations,&pop.diploids,wt,buffer);
      //Step 2: output fixations 
      unsigned temp = pop.fixations.size();
      buffer.write( reinterpret_cast<char*>(&temp), sizeof(unsigned) );
      std::for_each( pop.fixations.begin(), pop.fixations.end(),
		     std::bind(std::cref(wt),std::placeholders::_1,std::ref(buffer)) );
      //Step 3:the fixation times
      buffer.write( reinterpret_cast<const char *>(&pop.fixation_times[0]), pop.fixation_times.size()*sizeof(unsigned) );
    }
  public:
    mutable std::ostringstream buffer;
    template<typename sugarpop_t,
	     typename writer_t>
    inline typename std::enable_if<std::is_same<typename sugarpop_t::popmodel_t,sugar::SINGLEPOP_TAG>::value,result_type>::type
    operator()( const sugarpop_t & pop,
		const writer_t & wt ) const
    {
      buffer.str(std::string());
      buffer.write( reinterpret_cast<const char*>(&pop.N), sizeof(unsigned) );
      do_work(pop,wt);
    }

    template<typename sugarpop_t,
	     typename writer_t>
    inline typename std::enable_if<std::is_same<typename sugarpop_t::popmodel_t,sugar::METAPOP_TAG>::value,result_type>::type
    operator()( const sugarpop_t & pop,
		const writer_t & wt ) const
    {
      std::cerr << "this one\n";
      buffer.str(std::string());
      unsigned npops = pop.Ns.size();
      buffer.write(reinterpret_cast<char *>(&npops),sizeof(unsigned));
      buffer.write( reinterpret_cast<const char*>(&pop.Ns[0]), npops*sizeof(unsigned) );
      std::cerr << "here" << std::endl;
      do_work_metapop(pop,wt);
    }
			  
  };

  class deserialize
  {
  public:
    using result_type = void;
  private:
    template<typename sugarpop_t,
	     typename reader_t>
    result_type do_work( sugarpop_t & pop,
			 std::istringstream & i,
			 const reader_t & rt ) const
    {
      //Step 1: write the mutations, diploids, gametes to the stream
      KTfwd::read_binary_pop( &pop.gametes,&pop.mutations,&pop.diploids,rt,i );
      unsigned temp;
      i.read( reinterpret_cast<char*>(&temp),sizeof(unsigned) );
      for( unsigned m=0;m<temp ;++m )
    	{
    	  typename reader_t::result_type mm = rt(i);
    	  pop.fixations.emplace_back( std::move(mm) );
    	}
      pop.fixation_times.resize(temp);
      i.read( reinterpret_cast<char*>(&pop.fixation_times[0]), temp*sizeof(unsigned) );

      //Finally, fill the lookup table:
      std::for_each( pop.mutations.begin(), pop.mutations.end(),
    		     [&pop]( const typename sugarpop_t::mutation_t & __m ) { pop.mut_lookup.insert(__m.pos); } );
    }
    template<typename sugarpop_t,
	     typename reader_t>
    result_type do_work_metapop( sugarpop_t & pop,
				 std::istringstream & i,
				 const reader_t & rt ) const
    {
      //Step 1: write the mutations, diploids, gametes to the stream
      KTfwd::read_binary_metapop( &pop.gametes,&pop.mutations,&pop.diploids,rt,i );
      unsigned temp;
      i.read( reinterpret_cast<char*>(&temp),sizeof(unsigned) );
      for( unsigned m=0;m<temp ;++m )
    	{
    	  typename reader_t::result_type mm = rt(i);
    	  pop.fixations.emplace_back( std::move(mm) );
    	}
      pop.fixation_times.resize(temp);
      i.read( reinterpret_cast<char*>(&pop.fixation_times[0]), temp*sizeof(unsigned) );

      //Finally, fill the lookup table:
      std::for_each( pop.mutations.begin(), pop.mutations.end(),
    		     [&pop]( const typename sugarpop_t::mutation_t & __m ) { pop.mut_lookup.insert(__m.pos); } );
    }
  public:
    template<typename sugarpop_t,
	     typename reader_t>
    inline typename std::enable_if<std::is_same<typename sugarpop_t::popmodel_t,sugar::SINGLEPOP_TAG>::value,result_type>::type
    operator()( sugarpop_t & pop,
		const serialize & s,
		const reader_t & rt ) const
    {
      pop.clear();
      std::istringstream i(s.buffer.str());
      //Step 0: read N
      i.read( reinterpret_cast<char*>(&pop.N),sizeof(unsigned) );
      do_work(pop,i,rt);
    }

    template<typename sugarpop_t,
	     typename reader_t>
    inline typename std::enable_if<std::is_same<typename sugarpop_t::popmodel_t,sugar::METAPOP_TAG>::value,result_type>::type
    operator()( sugarpop_t & pop,
		const serialize & s,
		const reader_t & rt ) const
    {
      std::cerr << "this one reader!\n";
      pop.clear();
      std::istringstream i(s.buffer.str());
      //Step 0: read N
      unsigned numNs;
      i.read( reinterpret_cast<char*>(&numNs),sizeof(unsigned) );
      pop.Ns.resize(numNs);
      i.read( reinterpret_cast<char*>(&pop.Ns[0]),numNs*sizeof(unsigned) );
      std::cerr << numNs << '\n';
      do_work_metapop(pop,i,rt);
    }
  };
}

#endif
