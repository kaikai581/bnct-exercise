#!/usr/bin/env python

import argparse
import git
import os
import pandas as pd
import sys

# play with uproot4!
import uproot

class process_table:
    '''
    A class to manipulate the event number and nuclear process.
    '''
    def __init__(self, infpn, nentries=sys.maxsize):
        # open the file with uproot4
        # See the links in this website
        # https://opensourcelibs.com/lib/uproot4
        self.df = uproot.open('{}:nuclear_process'.format(infpn), library='pd').arrays(library='pd', entry_stop=nentries)

        # mark entries as redundant or not
        self.df['redundant'] = self.df['equation'].apply(self.mark_redundant_equation)
        self.sort_by_event_id_frequency(self.df)

        # dataframe with redundant equations removed
        self.df_cleanup = self.df[self.df.redundant == False].copy()
        self.sort_by_event_id_frequency(self.df_cleanup)

    def count_all_event_id(self, ascending=True):
        '''
        Count the frequency of all event ID.
        In principle, if data is producecd correctly, all IDs should be unique.
        '''
        return self.df['event_id'].value_counts(ascending=ascending)

    def count_all_processes(self, after_cleanup=True):
        '''
        Count the frequency of all nuclear processes involved in this simulation.
        '''
        if after_cleanup:
            df = self.df_cleanup
        else:
            df = self.df
        return self.df['equation'].value_counts()
    
    def get_all_processes(self, after_cleanup=True):
        '''
        Get all nuclear processes involved in this simulation.
        '''
        if after_cleanup:
            df = self.df_cleanup
        else:
            df = self.df
        return df['equation'].unique()
    
    def mark_redundant_equation(self, eqn_str):
        '''
        With the geant4 code I have, in a same event multiple nuclear reactions can happen.
        It is found many reactions are actually caused by secondaries, which shall be dropped.
        Besides, there are equations with only one daughter particle, such as
        proton + Be9 --> proton +
        This shall also be removed.
        '''
        wrong_parents = False
        empty_daughter = False
        parents, daughters = parse_equation(eqn_str)

        # test if there are parent particles other than proton and Be9
        if parents.difference(set(['proton', 'Be9'])):
            wrong_parents = True
        # test if there is an empty-string daughter
        if '' in daughters:
            empty_daughter = True
        # test if the interaction is elastic
        elastic = (parents == daughters)
        
        return wrong_parents or empty_daughter or elastic

    def sort_by_event_id_frequency(self, df, ascending=False):
        '''
        Sort data by event_id frequency.
        In principle, if data is producecd correctly, all IDs should be unique.
        '''
        df['id_count'] = df.groupby('event_id')['event_id'].transform('count')
        df = df.sort_values(['id_count','event_id'], ascending=ascending)
    
    def to_csv(self, outfpn):
        '''
        Save the original table to outfpn and the cleaned up one to (outfpn + 'cleanup').
        '''
        outdir = os.path.dirname(outfpn)
        if outdir and (not os.path.exists(outdir)):
            os.makedirs(outdir)
        # save the original table
        self.df.to_csv(outfpn)
        # save the cleaned up table
        fname, ext = os.path.splitext(outfpn)
        outfpn_clean = fname + '_cleaned' + ext
        self.df_cleanup.to_csv(outfpn_clean)

class secondary_particles:
    '''
    A class to skim secondary particle data.
    '''
    def __init__(self, infpn, nentries=sys.maxsize):
        # open the file with uproot4
        # See the links in this website
        # https://opensourcelibs.com/lib/uproot4
        self.df_sec = uproot.open('{}:secondary'.format(infpn), library='pd').arrays(library='pd', entry_stop=nentries)
        self.proc_tab = process_table(infpn)

        # augment the table to indicate what particles are involved in the production process
        # first step: leave only entries that are related to neutron production
        self.df_sec['neutron'] = self.df_sec['event_id'].map(self.df_sec.groupby('event_id').apply(lambda x: x['particle_name'].eq('neutron').any()))
        self.df_sec = self.df_sec[self.df_sec.neutron == True]

        # second step: extend the table columns to indicate relavant ions/particles
        for ion in ['B9', 'Be8', 'Be9', 'alpha', 'Li6', 'deuteron', 'B10']:
            self.df_sec[ion] = self.df_sec['event_id'].map(self.df_sec.groupby('event_id').apply(lambda x: x['particle_name'].eq(ion).any()))
        
        # adding the nuclear process column by looking up in the process table with the same event id
        # ref: https://realpython.com/pandas-merge-join-and-concat/#:~:text=left_on%20and%20right_on%20%3A%20Use%20either,Both%20default%20to%20False%20.
        self.df_sec = pd.merge(self.df_sec, self.proc_tab.df_cleanup, on='event_id')
    
    def get_all_particles(self):
        '''
        Get all particle species involved in this simulation.
        One invokation gave me the following result.
        ['neutron' 'B9' 'proton' 'Be8' 'e-' 'gamma' 'Be9' 'N14' 'O16' 'alpha', 'Li6' 'deuteron' 'B10' 'C14' 'N15' 'Ar40' 'C12']
        '''
        return self.df_sec.particle_name.unique()
    
    def to_csv(self, outfpn, nentries=sys.maxsize):
        '''
        Output a human readable table.
        '''
        outdir = os.path.dirname(outfpn)
        if outdir and (not os.path.exists(outdir)):
            os.makedirs(outdir)
        # save the original table
        self.df_sec.head(nentries).to_csv(outfpn)
        
def get_git_root(path):
    '''
    Get the root directory of this project.
    '''
    git_repo = git.Repo(path, search_parent_directories=True)
    git_root = git_repo.git.rev_parse('--show-toplevel')
    return git_root

def parse_equation(eqn_str):
    '''
    A helper function to parse reaction equations.
    '''
    sides_of_eqn = eqn_str.split('-->')
    lhs = set(map(str.strip, sides_of_eqn[0].split('+')))
    rhs = set(map(str.strip, sides_of_eqn[1].split('+')))
    return lhs, rhs

if __name__ == '__main__':
    # command line arguments
    parser = argparse.ArgumentParser()
    parser.add_argument('-i', '--input_file', type=str, default=os.path.join(get_git_root(__file__), 'data/p_on_be/root/p_on_be_pencil_6MeV_r6cm_t1mm_v4.root'))
    parser.add_argument('-o', '--output_directory', type=str, default=os.path.join(get_git_root(__file__), 'data/p_on_be/pandas'))
    args = parser.parse_args()
    infpn = args.input_file
    outdir = args.output_directory

    # configure pandas to print all rows
    pd.set_option('display.max_rows', None)

    # some debugging information
    # ptable = process_table(infpn)
    # print(ptable.df_cleanup['equation'].unique())

    data = secondary_particles(infpn)
    data.to_csv(os.path.join(outdir, 'neutron_preview.csv'))