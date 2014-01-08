//Copyright (c) 2006-2009 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.lslboost.org/LICENSE_1_0.txt)

#ifndef UUID_618474C2DE1511DEB74A388C56D89593
#define UUID_618474C2DE1511DEB74A388C56D89593
#if defined(__GNUC__) && !defined(BOOST_EXCEPTION_ENABLE_WARNINGS)
#pragma GCC system_header
#endif
#if defined(_MSC_VER) && !defined(BOOST_EXCEPTION_ENABLE_WARNINGS)
#pragma warning(push,1)
#endif

#include <lslboost/config.hpp>
#ifdef BOOST_NO_EXCEPTIONS
#error This header requires exception handling to be enabled.
#endif
#include <lslboost/exception/exception.hpp>
#include <lslboost/exception/info.hpp>
#include <lslboost/exception/diagnostic_information.hpp>
#include <lslboost/exception/detail/type_info.hpp>
#include <lslboost/exception/detail/clone_current_exception.hpp>
#include <lslboost/shared_ptr.hpp>
#include <stdexcept>
#include <new>
#include <ios>
#include <stdlib.h>

namespace
lslboost
    {
    class exception_ptr;
    BOOST_ATTRIBUTE_NORETURN void rethrow_exception( exception_ptr const & );
    exception_ptr current_exception();

    class
    exception_ptr
        {
        typedef lslboost::shared_ptr<exception_detail::clone_base const> impl;
        impl ptr_;
        friend void rethrow_exception( exception_ptr const & );
        typedef exception_detail::clone_base const * (impl::*unspecified_bool_type)() const;
        public:
        exception_ptr()
            {
            }
        explicit
        exception_ptr( impl const & ptr ):
            ptr_(ptr)
            {
            }
        bool
        operator==( exception_ptr const & other ) const
            {
            return ptr_==other.ptr_;
            }
        bool
        operator!=( exception_ptr const & other ) const
            {
            return ptr_!=other.ptr_;
            }
        operator unspecified_bool_type() const
            {
            return ptr_?&impl::get:0;
            }
        };

    template <class T>
    inline
    exception_ptr
    copy_exception( T const & e )
        {
        try
            {
            throw enable_current_exception(e);
            }
        catch(
        ... )
            {
            return current_exception();
            }
        }

#ifndef BOOST_NO_RTTI
    typedef error_info<struct tag_original_exception_type,std::type_info const *> original_exception_type;

    inline
    std::string
    to_string( original_exception_type const & x )
        {
        return x.value()->name();
        }
#endif

    namespace
    exception_detail
        {
        struct
        bad_alloc_:
            lslboost::exception,
            std::bad_alloc
                {
                ~bad_alloc_() throw() { }
                };

        struct
        bad_exception_:
            lslboost::exception,
            std::bad_exception
                {
                ~bad_exception_() throw() { }
                };

        template <class Exception>
        exception_ptr
        get_static_exception_object()
            {
            Exception ba;
            exception_detail::clone_impl<Exception> c(ba);
            c <<
                throw_function(BOOST_CURRENT_FUNCTION) <<
                throw_file(__FILE__) <<
                throw_line(__LINE__);
            static exception_ptr ep(shared_ptr<exception_detail::clone_base const>(new exception_detail::clone_impl<Exception>(c)));
            return ep;
            }

        template <class Exception>
        struct
        exception_ptr_static_exception_object
            {
            static exception_ptr const e;
            };

        template <class Exception>
        exception_ptr const
        exception_ptr_static_exception_object<Exception>::
        e = get_static_exception_object<Exception>();
        }

#if defined(__GNUC__)
# if (__GNUC__ == 4 && __GNUC_MINOR__ >= 1) || (__GNUC__ > 4)
#  pragma GCC visibility push (default)
# endif
#endif
    class
    unknown_exception:
        public lslboost::exception,
        public std::exception
        {
        public:

        unknown_exception()
            {
            }

        explicit
        unknown_exception( std::exception const & e )
            {
            add_original_type(e);
            }

        explicit
        unknown_exception( lslboost::exception const & e ):
            lslboost::exception(e)
            {
            add_original_type(e);
            }

        ~unknown_exception() throw()
            {
            }

        private:

        template <class E>
        void
        add_original_type( E const & e )
            {
#ifndef BOOST_NO_RTTI
            (*this) << original_exception_type(&typeid(e));
#endif
            }
        };
#if defined(__GNUC__)
# if (__GNUC__ == 4 && __GNUC_MINOR__ >= 1) || (__GNUC__ > 4)
#  pragma GCC visibility pop
# endif
#endif

    namespace
    exception_detail
        {
        template <class T>
        class
        current_exception_std_exception_wrapper:
            public T,
            public lslboost::exception
            {
            public:

            explicit
            current_exception_std_exception_wrapper( T const & e1 ):
                T(e1)
                {
                add_original_type(e1);
                }

            current_exception_std_exception_wrapper( T const & e1, lslboost::exception const & e2 ):
                T(e1),
                lslboost::exception(e2)
                {
                add_original_type(e1);
                }

            ~current_exception_std_exception_wrapper() throw()
                {
                }

            private:

            template <class E>
            void
            add_original_type( E const & e )
                {
#ifndef BOOST_NO_RTTI
                (*this) << original_exception_type(&typeid(e));
#endif
                }
            };

#ifdef BOOST_NO_RTTI
        template <class T>
        lslboost::exception const *
        get_lslboost_exception( T const * )
            {
            try
                {
                throw;
                }
            catch(
            lslboost::exception & x )
                {
                return &x;
                }
            catch(...)
                {
                return 0;
                }
            }
#else
        template <class T>
        lslboost::exception const *
        get_lslboost_exception( T const * x )
            {
            return dynamic_cast<lslboost::exception const *>(x);
            }
#endif

        template <class T>
        inline
        exception_ptr
        current_exception_std_exception( T const & e1 )
            {
            if( lslboost::exception const * e2 = get_lslboost_exception(&e1) )
                return lslboost::copy_exception(current_exception_std_exception_wrapper<T>(e1,*e2));
            else
                return lslboost::copy_exception(current_exception_std_exception_wrapper<T>(e1));
            }

        inline
        exception_ptr
        current_exception_unknown_exception()
            {
            return lslboost::copy_exception(unknown_exception());
            }

        inline
        exception_ptr
        current_exception_unknown_lslboost_exception( lslboost::exception const & e )
            {
            return lslboost::copy_exception(unknown_exception(e));
            }

        inline
        exception_ptr
        current_exception_unknown_std_exception( std::exception const & e )
            {
            if( lslboost::exception const * be = get_lslboost_exception(&e) )
                return current_exception_unknown_lslboost_exception(*be);
            else
                return lslboost::copy_exception(unknown_exception(e));
            }

        inline
        exception_ptr
        current_exception_impl()
            {
            exception_detail::clone_base const * e=0;
            switch(
            exception_detail::clone_current_exception(e) )
                {
                case exception_detail::clone_current_exception_result::
                success:
                    {
                    BOOST_ASSERT(e!=0);
                    return exception_ptr(shared_ptr<exception_detail::clone_base const>(e));
                    }
                case exception_detail::clone_current_exception_result::
                bad_alloc:
                    {
                    BOOST_ASSERT(!e);
                    return exception_detail::exception_ptr_static_exception_object<bad_alloc_>::e;
                    }
                case exception_detail::clone_current_exception_result::
                bad_exception:
                    {
                    BOOST_ASSERT(!e);
                    return exception_detail::exception_ptr_static_exception_object<bad_exception_>::e;
                    }
                default:
                    BOOST_ASSERT(0);
                case exception_detail::clone_current_exception_result::
                not_supported:
                    {
                    BOOST_ASSERT(!e);
                    try
                        {
                        throw;
                        }
                    catch(
                    exception_detail::clone_base & e )
                        {
                        return exception_ptr(shared_ptr<exception_detail::clone_base const>(e.clone()));
                        }
                    catch(
                    std::domain_error & e )
                        {
                        return exception_detail::current_exception_std_exception(e);
                        }
                    catch(
                    std::invalid_argument & e )
                        {
                        return exception_detail::current_exception_std_exception(e);
                        }
                    catch(
                    std::length_error & e )
                        {
                        return exception_detail::current_exception_std_exception(e);
                        }
                    catch(
                    std::out_of_range & e )
                        {
                        return exception_detail::current_exception_std_exception(e);
                        }
                    catch(
                    std::logic_error & e )
                        {
                        return exception_detail::current_exception_std_exception(e);
                        }
                    catch(
                    std::range_error & e )
                        {
                        return exception_detail::current_exception_std_exception(e);
                        }
                    catch(
                    std::overflow_error & e )
                        {
                        return exception_detail::current_exception_std_exception(e);
                        }
                    catch(
                    std::underflow_error & e )
                        {
                        return exception_detail::current_exception_std_exception(e);
                        }
                    catch(
                    std::ios_base::failure & e )
                        {
                        return exception_detail::current_exception_std_exception(e);
                        }
                    catch(
                    std::runtime_error & e )
                        {
                        return exception_detail::current_exception_std_exception(e);
                        }
                    catch(
                    std::bad_alloc & e )
                        {
                        return exception_detail::current_exception_std_exception(e);
                        }
#ifndef BOOST_NO_TYPEID
                    catch(
                    std::bad_cast & e )
                        {
                        return exception_detail::current_exception_std_exception(e);
                        }
                    catch(
                    std::bad_typeid & e )
                        {
                        return exception_detail::current_exception_std_exception(e);
                        }
#endif
                    catch(
                    std::bad_exception & e )
                        {
                        return exception_detail::current_exception_std_exception(e);
                        }
                    catch(
                    std::exception & e )
                        {
                        return exception_detail::current_exception_unknown_std_exception(e);
                        }
                    catch(
                    lslboost::exception & e )
                        {
                        return exception_detail::current_exception_unknown_lslboost_exception(e);
                        }
                    catch(
                    ... )
                        {
                        return exception_detail::current_exception_unknown_exception();
                        }
                    }
                }
            }
        }

    inline
    exception_ptr
    current_exception()
        {
        exception_ptr ret;
        try
            {
            ret=exception_detail::current_exception_impl();
            }
        catch(
        std::bad_alloc & )
            {
            ret=exception_detail::exception_ptr_static_exception_object<exception_detail::bad_alloc_>::e;
            }
        catch(
        ... )
            {
            ret=exception_detail::exception_ptr_static_exception_object<exception_detail::bad_exception_>::e;
            }
        BOOST_ASSERT(ret);
        return ret;
        }

    BOOST_ATTRIBUTE_NORETURN
    inline
    void
    rethrow_exception( exception_ptr const & p )
        {
        BOOST_ASSERT(p);
        p.ptr_->rethrow();
        BOOST_ASSERT(0);
        #if defined(UNDER_CE)
            // some CE platforms don't define ::abort()
            exit(-1);
        #else
            abort();
        #endif
        }

    inline
    std::string
    diagnostic_information( exception_ptr const & p )
        {
        if( p )
            try
                {
                rethrow_exception(p);
                }
            catch(
            ... )
                {
                return current_exception_diagnostic_information();
                }
        return "<empty>";
        }

    inline
    std::string
    to_string( exception_ptr const & p )
        {
        std::string s='\n'+diagnostic_information(p);
        std::string padding("  ");
        std::string r;
        bool f=false;
        for( std::string::const_iterator i=s.begin(),e=s.end(); i!=e; ++i )
            {
            if( f )
                r+=padding;
            char c=*i;
            r+=c;
            f=(c=='\n');
            }
        return r;
        }
    }

#if defined(_MSC_VER) && !defined(BOOST_EXCEPTION_ENABLE_WARNINGS)
#pragma warning(pop)
#endif
#endif