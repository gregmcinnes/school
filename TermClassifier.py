import mysql.connector
import re
import argparse
import os

global cursor
db = mysql.connector.connect(
    user='gmcinnes',
    database='umls',
    password='password',
    host='localhost')
cursor = db.cursor()

MESH_DEFINITIONS = {
    'disease': 'D',
    'drug': 'C',
    'gene': 'G14325'
}

# Link outs
# Diseases
# http://www.ncbi.nlm.nih.gov/gtr/conditions/C0011849/

def main(file, try_hard=False):

    basename = os.path.basename(file)
    rootname = basename.split('.')[0]

    output = rootname + ".classifications.tsv"
    unclassified = rootname + ".unclassified.tsv"

    terms = []
    with open(file) as f:
        for line in f:
            array = line.split(',')
            for t in array:
                t = ''.join(t.split())
                if t not in terms:
                    terms.append(t)
    classified_file = open(output, 'w')
    unclassified_file = open(unclassified, 'w')

    for t in terms:
        classification, cui, mesh_tree_number = classify_term(t, try_hard)
        cui_string = join_list(cui)
        mesh_tree_number_string = join_list(mesh_tree_number)
        if classification is not None:
            classified_file.write("%s\t%s\t%s\t%s\n" % (t, classification, cui_string, mesh_tree_number_string))
        else:
            unclassified_file.write("%s\t%s\t%s\n" % (t, cui_string, mesh_tree_number_string))
    classified_file.close()

def join_list(list):
    if list is not None:
        return (','.join(list))
    return "None"

def classify_term(term, try_hard=False):
    classification = check_snp(term)
    if classification is not None:
        return classification, None, None

    cui = get_cui(term, try_hard)
    mesh_tree_number = None

    if cui is not None:
        mesh_tree_number = get_mesh_tree_number(cui)

    if mesh_tree_number is not None:
        classification = classify_mesh_tree_number(mesh_tree_number)

    gene = check_gene(term)
    if gene is not None:
        classification = gene

    return classification, cui, mesh_tree_number

def check_snp(term):
    # Is it a SNP?
    regex = r'^rs\d*$'
    if re.match(regex, term):
        return "SNP"
    return None

def check_gene(term):
    gene_file = '/Users/gmcinnes/bmi210/gene_list.txt'
    with open(gene_file) as f:
        for line in f:
            gene_name = " ".join(line.split())
            if term.lower() == gene_name.lower():
                return "gene"
    return None

def classify_mesh_tree_number(mesh_tree_number):
    # todo rewrite to check from most specific to least specific once all classifications have been identified
    classifications = {
        "specific": {
            "symptom": ["C23.888"],
            "gene": ["G05.360"],
            "gene_product": ["D12.776"]
        },
        "general": {
            "disease": ["C","F03"],
            "drug": ["D"],

        }
    }
    result = None
    for tree_number in mesh_tree_number:
        for c in classifications["specific"]:
            for s in classifications["specific"][c]:
                regex = r"^%s" % s
                if re.search(regex, str(tree_number), re.IGNORECASE):
                    if result is None:
                        result = c
                        return result
                    elif result != c:
                        print "Multiple MESH tree number in classification disagreement"
                        return None

        for c in classifications["general"]:
            for s in classifications["general"][c]:
                regex = r"^%s" % s
                if re.search(regex, str(tree_number), re.IGNORECASE):
                    if result is None:
                        result = c
                        return result
                    elif result != c:
                        print "Multiple MESH tree number in classification disagreement"
                        return None
    return result

def get_mesh_tree_number(cui):
    result = mesh_tree_number_query(cui)
    tree_number = check_result(result)
    return tree_number

def get_cui(term, try_hard=False):
    # Is this a MESH term?
    cui = check_term(term, "MSH")
    if cui is not None:
        return cui

    # Is it a SNOMEDCT term?
    cui = check_term(term, "SNOMEDCT")
    if cui is not None:
        return cui

    # Is it anything else?
    cui = check_term(term)
    if cui is not None:
        return cui

    # Was it hypenated?
    if try_hard is True:
        hyphenated = hyphenate(term)
        if hyphenated is not None:
            print hyphenated
            cui = check_term(hyphenated)
            return cui

    # Maybe it's part of another term?
    #if try_hard is True and len(term) > 3:
    #    cui = check_term(term, like=True)
    #    return cui
    return cui

def hyphenate(term):
    hyphened = None
    hyphen_file = '/Users/gmcinnes/bmi210/hyphenated_to_nonhyphenated_mapped.txt'
    with open(hyphen_file) as f:
        for line in f:
            array = line.split('\t')
            if array[0] == term:
                hyphened = array[1]
                return hyphened
    return hyphened

def check_term(str, sab=None, like=False):
    result = cui_query(str=str, sab=sab, like=like)
    cui = check_result(result)
    return cui

def cui_query(str=None, sab=None, like=False):
    global cursor
    query = """
            SELECT
                CUI,
                AUI,
                CODE,
                STR
            FROM
                MRCONSO
            WHERE
            """
    if like is True:
        query += """
            STR like '%%%s%%'
            """ % str
    else:
        query += """
                STR='%s'
            """ % str
    if sab is not None:
        query += """
                AND SAB='%s';
                """ % (sab)

    cursor.execute(query)
    return cursor.fetchall()

def mesh_tree_number_query(cui_list):
    global cursor
    cui_string = (', '.join('"' + cui + '"' for cui in cui_list))

    query = """
        SELECT DISTINCT
            ATV
        FROM
            MRSAT
        WHERE
            ATN='MN'
            AND CUI IN (%s)
        """ % cui_string
    cursor.execute(query)
    return cursor.fetchall()

def check_result(result):
    if len(result) is 0:
        return None
    else:
        cui_list = []
        for row in result :
            cui = row[0]
            if cui not in cui_list:
                cui_list.append(row[0])
        return cui_list

def parse_command_line():
    parser = argparse.ArgumentParser(
        description = 'This script attempts to classify terms using UMLS.')

    parser.add_argument("--input", type=str, default=None,
                                help="Input file.  A csv file with terms to be classified.")
    parser.add_argument("--try_hard",  default=False, action="store_true",
                                help="Try extra hard to classify terms.  WARNING: Slow and inaccurate.")
    options = parser.parse_args()
    if options.input is None:
        print "Input file is required to proceed.  Use --input to specify an input file."
        exit(0)
    return options

if __name__ == "__main__":
    options = parse_command_line()

    main(file=options.input, try_hard=options.try_hard)
