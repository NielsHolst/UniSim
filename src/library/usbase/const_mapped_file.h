/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_CONST_MAPPED_FILE_H
#define UNISIM_CONST_MAPPED_FILE_H
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/range/iterator_range.hpp>

namespace UniSim{


class ConstMappedFile
{
public:
    typedef const char* Iterator;
    typedef boost::iterator_range<Iterator> IteratorRange;

    ConstMappedFile() {}
    bool open(const std::string& filePath) { file.open(filePath); return file.is_open(); }

    Iterator      begin() const { return file.data(); }
    Iterator        end() const { return file.data() + file.size(); }
    IteratorRange range() const { return IteratorRange(begin(), end()); }

private:
    boost::iostreams::mapped_file_source file;
};

} //namespace
#endif
