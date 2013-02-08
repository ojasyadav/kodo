// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#ifndef KODO_RANDOM_UNIFORM_SYMBOL_ID_HPP
#define KODO_RANDOM_UNIFORM_SYMBOL_ID_HPP

#include <cstdint>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#include <fifi/fifi_utils.hpp>

namespace kodo
{

    /// @brief Generates uniformly random coding coefficients and write
    ///        all coefficients to the symbol id buffer.
    ///
    /// @ingroup symbol_id_layers
    /// @ingroup factory_layers
    template<class SuperCoder>
    class random_uniform_symbol_id : public SuperCoder
    {
    public:

        /// The random generator used
        typedef boost::random::mt19937 generator_type;

        /// The type generated by this random generator
        typedef generator_type::result_type result_type;

        /// The value type
        typedef typename SuperCoder::value_type value_type;

        /// The finite field type
        typedef typename SuperCoder::field_type field_type;

    public:

        /// The factory layer associated with this coder.
        class factory : public SuperCoder::factory
        {
        public:

            /// @layer_factory{factory}
            factory(uint32_t max_symbols, uint32_t max_symbol_size);

            /// @layer_factory{max_symbol_id_size}
            uint32_t max_symbol_id_size() const;
        };

    public:

        /// @layer{initialize}
        void initialize(uint32_t symbols, uint32_t symbol_size);

        /// @layer{write_id}
        void write_id(uint8_t *symbol_id, uint8_t **symbol_id_coefficients);

        /// @layer{read_id}
        void read_id(uint8_t *symbol_id, uint8_t **symbol_id_coefficients);

        /// @layer{symbol_id_size}
        uint32_t symbol_id_size() const;

    protected:

        /// Seeds the random generator used
        /// @param seed The seed used for the random generator
        void seed(result_type seed);

    protected:

        /// The distribution wrapping the random generator
        boost::random::uniform_int_distribution<uint8_t> m_distribution;

        /// The random generator
        generator_type m_random_generator;

        /// The number of bytes needed to store the symbol id
        /// coding coefficients
        uint32_t m_id_size;

    };


    template<class SuperCoder>
    inline random_uniform_symbol_id<SuperCoder>::factory::factory(
        uint32_t max_symbols,
        uint32_t max_symbol_size)
        : SuperCoder::factory(max_symbols, max_symbol_size)
    { }


    template<class SuperCoder>
    inline uint32_t
    random_uniform_symbol_id<SuperCoder>::factory::max_symbol_id_size() const
    {
        uint32_t max_symbol_id_size =
            fifi::bytes_needed<field_type>(
                SuperCoder::factory::max_symbols());

        assert(max_symbol_id_size > 0);

                    return max_symbol_id_size;
    }


    template<class SuperCoder>
    inline void random_uniform_symbol_id<SuperCoder>::initialize(
        uint32_t symbols,
        uint32_t symbol_size)
    {
        SuperCoder::initialize(symbols, symbol_size);

        m_id_size = fifi::bytes_needed<field_type>(symbols);
        assert(m_id_size > 0);
    }


    template<class SuperCoder>
    inline void random_uniform_symbol_id<SuperCoder>::write_id(
        uint8_t *symbol_id,
        uint8_t **symbol_id_coefficients)
    {
        assert(symbol_id != 0);
        assert(symbol_id_coefficients != 0);

        for(uint32_t i = 0; i < m_id_size; ++i)
        {
            symbol_id[i] = m_distribution(m_random_generator);
        }

        *symbol_id_coefficients = symbol_id;
    }


    template<class SuperCoder>
    inline void random_uniform_symbol_id<SuperCoder>::read_id(
        uint8_t *symbol_id,
        uint8_t **symbol_id_coefficients)
    {
        assert(symbol_id != 0);
        assert(symbol_id_coefficients != 0);

        *symbol_id_coefficients = symbol_id;
    }


    template<class SuperCoder>
    inline uint32_t random_uniform_symbol_id<SuperCoder>::symbol_id_size() const
    {
        return m_id_size;
    }


    template<class SuperCoder>
    inline void random_uniform_symbol_id<SuperCoder>::seed(result_type seed)
    {
        return m_random_generator.seed(seed);
    }

}

#endif

