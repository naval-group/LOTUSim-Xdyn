#include "Hdf5Observer.hpp"

#include "h5_version.hpp"
#include "h5_tools.hpp"
#include "h5_tools.hpp"
#include "stl_io_hdf5.hpp"

#include "demoMatLab.hpp"
#include "demoPython.hpp"

#include "Hdf5WaveObserver.hpp"
#include "InternalErrorException.hpp"
#include "Hdf5WaveSpectrumObserver.hpp"

Hdf5Addressing::Hdf5Addressing(
        const DataAddressing& addressing,
        const std::string& basename) :
            address(H5_Tools::ensureStringStartsWithAPattern(basename,"/") +
                    H5_Tools::ensureStringStartsWithAPattern(H5_Tools::join(addressing.address,"/"),"/"))
{
}

Hdf5Observer::Hdf5Observer(const std::string& filename_) :
            Observer(),
            h5File(H5_Tools::openEmptyHdf5File(filename_)),
            basename("outputs"),
            name2address(),
            name2dataset(),
            name2datatype(),
            name2dataspace(),
            wave_serializer(),
            filename(filename_)
{
    h5_writeFileDescription(h5File);
}

Hdf5Observer::Hdf5Observer(
        const std::string& filename_,
        const std::vector<std::string>& d) :
            Observer(d),
            h5File(H5_Tools::openEmptyHdf5File(filename_)),
            basename("outputs"),
            name2address(),
            name2dataset(),
            name2datatype(),
            name2dataspace(),
            wave_serializer(),
            filename(filename_)
{
    h5_writeFileDescription(h5File);
}

std::function<void()> Hdf5Observer::get_serializer(const double val, const DataAddressing& addressing)
{
    return [this,val,addressing]()
           {
                H5::DataSet dataset = name2dataset[addressing.name];
                H5::DataSpace dataspace = dataset.getSpace();
                hsize_t offset[1] = {(hsize_t)0};
                hsize_t size[1] = {(hsize_t)0};
                if (dataspace.getSimpleExtentDims(size)!=1)
                {
                    std::stringstream ss;
                    ss << "Rank mismatch -> Should be one, not " << dataspace.getSimpleExtentNdims();
                    THROW(__PRETTY_FUNCTION__, InternalErrorException, ss.str());
                }
                const hsize_t dims[1] = {(hsize_t)1};
                offset[0] = size[0];
                size[0] += 1;
                dataset.extend(size);
                H5::DataSpace fspace = dataset.getSpace();
                fspace.selectHyperslab(H5S_SELECT_SET, dims, offset);
                dataset.write(&val, name2datatype[addressing.name], H5_Tools::createDataSpace1DUnlimited(), fspace);
           };
}

std::function<void()> Hdf5Observer::get_initializer(const double , const DataAddressing& addressing)
{
    return [this,addressing]()
           {
                name2address[addressing.name] = Hdf5Addressing(addressing,this->basename).address;
                name2datatype[addressing.name] = H5::DataType(H5::PredType::NATIVE_DOUBLE);
                name2dataspace[addressing.name] = H5_Tools::createDataSpace1DEmptyUnlimited();
                name2dataset[addressing.name] =
                        H5_Tools::createDataSet(h5File,
                                                name2address[addressing.name],
                                                name2datatype[addressing.name],
                                                name2dataspace[addressing.name]);
           };
}

std::function<void()> Hdf5Observer::get_serializer(const SurfaceElevationGrid& waveElevationGrid, const DataAddressing&)
{
    return [this,waveElevationGrid](){(*wave_serializer)<<waveElevationGrid;};
}

std::function<void()> Hdf5Observer::get_initializer(const SurfaceElevationGrid& waveElevationGrid, const DataAddressing& addressing)
{
    return [this,waveElevationGrid, addressing]()
           {
               const size_t nx = (size_t)waveElevationGrid.x.size();
               const size_t ny = (size_t)waveElevationGrid.y.size();
               wave_serializer = Hdf5WaveObserverPtr(new Hdf5WaveObserver(h5File, this->basename+"/waves", nx, ny));
           };
}

void Hdf5Observer::flush_after_initialization()
{
}

void Hdf5Observer::flush_after_write()
{
}

void Hdf5Observer::flush_value_during_write()
{
}

void Hdf5Observer::write_before_simulation(const std::vector<FlatDiscreteDirectionalWaveSpectrum>& s, const DataAddressing&)
{
    if (should_serialize("spectra"))
    {
        hdf5WaveSpectrumObserver(h5File,"/outputs/spectra", s);
        // Should only be serialized at the beginning of the simulation, otherwise xdyn
        // will attempt to serialize it at each timestep & will fail
        remove_variable("spectra");
    }
}

void Hdf5Observer::write_command_line_before_simulation(const std::string& command_line)
{
    if (should_serialize("command line"))
    {
        write_before_simulation(command_line, DataAddressing({"command"}, "CLI command"));
        // Should only be serialized at the beginning of the simulation, otherwise xdyn
        // will attempt to serialize it at each timestep & will fail
        remove_variable("command line");
    }
}

void Hdf5Observer::write_yaml_before_simulation(const std::string& yaml)
{
    if (should_serialize("yaml"))
    {
        write_before_simulation(yaml, DataAddressing({"yaml","input"}, "YAML input"));
        // Should only be serialized at the beginning of the simulation, otherwise xdyn
        // will attempt to serialize it at each timestep & will fail
        remove_variable("yaml");
    }
}

void Hdf5Observer::write_matlab_script_before_simulation()
{

    if (should_serialize("matlab scripts"))
    {
        exportMatLabScripts(h5File, filename, basename, "/scripts/MatLab");
        // Should only be serialized at the beginning of the simulation, otherwise xdyn
        // will attempt to serialize it at each timestep & will fail
        remove_variable("matlab scripts");
    }
}

void Hdf5Observer::write_python_script_before_simulation()
{
    if (should_serialize("python scripts"))
    {
        exportPythonScripts(h5File, filename, basename, "/scripts/Python");
        // Should only be serialized at the beginning of the simulation, otherwise xdyn
        // will attempt to serialize it at each timestep & will fail
        remove_variable("python scripts");
    }
}

void Hdf5Observer::write_before_simulation(const MeshPtr mesh, const DataAddressing& address)
{
    if (mesh->nb_of_static_nodes>0)
    {
        writeMeshToHdf5File(h5File, Hdf5Addressing(address, "inputs").address, mesh->nodes, mesh->facets);
    }
}


void Hdf5Observer::write_before_simulation(const std::string& data, const DataAddressing& address)
{
    if(not(data.empty()))
    {
        H5_Tools::write(h5File, Hdf5Addressing(address, "inputs").address, data);
    }
    
}